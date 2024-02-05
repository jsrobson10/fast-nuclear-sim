
#include "mesh.hpp"
#include "arrays.hpp"
#include "../shader.hpp"
#include "../camera.hpp"
#include "../input/focus.hpp"
#include "../../util/math.hpp"

#include <iostream>

using namespace sim::graphics;

void mesh::add(const mesh& o, glm::mat4 mat)
{
	unsigned int off = vertices.size();
	glm::mat3 mat3(mat);

	vertices.reserve(vertices.size() + o.vertices.size());
	indices.reserve(indices.size() + o.indices.size());

	for(unsigned int i = 0; i < o.vertices.size(); i++)
	{
		arrays::vertex v = o.vertices[i];
		v.normal = v.normal * mat3;
		v.pos = v.pos * mat;
		vertices.push_back(v);
	}

	for(unsigned int i = 0; i < o.indices.size(); i++)
	{
		indices.push_back(o.indices[i] + off);
	}
}

void mesh::set_vertices(const arrays::vertex* data, size_t size)
{
	vertices.clear();
	vertices.reserve(size);

	for(unsigned int i = 0; i < size; i++)
	{
		vertices.push_back(data[i]);
	}
}

void mesh::set_indices(const unsigned int* data, size_t size)
{
	indices.clear();
	indices.reserve(size);

	for(unsigned int i = 0; i < size; i++)
	{
		indices.push_back(data[i]);
	}
}

typedef glm::vec<3, double> vec3;

bool ray_intersects_triangle(vec3 ray_origin,
                             vec3 ray_vector,
                             const vec3* triangle,
                             vec3& out_intersection_point)
{
    constexpr double epsilon = std::numeric_limits<double>::epsilon();

    vec3 edge1 = triangle[1] - triangle[0];
    vec3 edge2 = triangle[2] - triangle[0];
    vec3 ray_cross_e2 = cross(ray_vector, edge2);
    double det = dot(edge1, ray_cross_e2);

    if (det > -epsilon && det < epsilon)
        return false;    // This ray is parallel to this triangle.

    double inv_det = 1.0 / det;
    vec3 s = ray_origin - triangle[0];
    double u = inv_det * dot(s, ray_cross_e2);

    if (u < 0 || u > 1)
        return false;

    vec3 s_cross_e1 = cross(s, edge1);
    double v = inv_det * dot(ray_vector, s_cross_e1);

    if (v < 0 || u + v > 1)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    double t = inv_det * dot(edge2, s_cross_e1);
    out_intersection_point = ray_origin + ray_vector * t;

    if (t > epsilon) // ray intersection
    {
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

bool mesh::check_focus(double len) const
{
	auto near = focus::get_trigger_near();
	auto far = focus::get_trigger_far();
	
	return focus::is_triggered() && check_intersect(near, glm::normalize(far - near) * len);
}

bool mesh::check_focus() const
{
	return check_focus(2.5);
}

bool mesh::check_intersect(vec3 pos, vec3 path) const
{
	double l = glm::length(path);

	if(l == 0)
	{
		return false;
	}

	vec3 path_n = path / l;
	
	for(unsigned int i = 0; i < indices.size(); i += 3)
	{
		vec3 v[3] = {
			vec3(this->vertices[indices[i]].pos),
			vec3(this->vertices[indices[i + 1]].pos),
			vec3(this->vertices[indices[i + 2]].pos)
		};
		
		vec3 ipoint;
		vec3 normal = glm::normalize(glm::cross(v[1] - v[0], v[2] - v[0]));
		double d = glm::dot(normal, path);

		if(d >= 0)
			continue;
		if(!ray_intersects_triangle(pos, path_n, v, ipoint))
			continue;
		if(l < glm::length(ipoint - pos))
			continue;
		
		return true;
	}

	return false;
}

vec3 mesh::calc_intersect(vec3 pos, vec3 path) const
{
	vec3 normal_last(0);
	return calc_intersect(pos, path, normal_last);
}

static bool calc_intercept_vert(vec3 v[3], vec3 pos, vec3& path, vec3& path_n, vec3& normal_last, double& l)
{
	vec3 ipoint;
	vec3 normal = glm::normalize(glm::cross(v[1] - v[0], v[2] - v[0]));
	double d = glm::dot(normal, path);

	if(d >= 0)
		return false;
	if(!ray_intersects_triangle(pos, path_n, v, ipoint))
		return false;
	if(l < glm::length(ipoint - pos))
		return false;

	if(normal_last != vec3(0))
	{
		vec3 n = glm::cross(normal_last, normal);

		if(glm::length(n) > 0)
		{
			normal = glm::normalize(glm::cross(glm::cross(normal_last, normal), normal_last));
			d = glm::dot(normal, path);
		}
	}
	
	path -= normal * d;
	normal_last = normal;
	l = glm::length(path);

	if(l > 0)
	{
		path_n = path / l;
	}

	return true;
}

vec3 mesh::calc_intersect(vec3 pos, vec3 path, vec3& normal_last) const
{
	double l = glm::length(path);

	if(l == 0)
	{
		return path;
	}

	vec3 path_n = path / l;
	unsigned int i_found = 0;

	for(unsigned int i = 0; i < indices.size(); i += 3)
	{
		vec3 v[3] = {
			vec3(this->vertices[indices[i]].pos),
			vec3(this->vertices[indices[i + 1]].pos),
			vec3(this->vertices[indices[i + 2]].pos)
		};
		
		if(calc_intercept_vert(v, pos, path, path_n, normal_last, l))
		{
			i_found = i;
		}

		if(l == 0)
		{
			return path;
		}
	}

	for(unsigned int i = 0; i < i_found; i += 3)
	{
		vec3 v[3] = {
			vec3(this->vertices[indices[i]].pos),
			vec3(this->vertices[indices[i + 1]].pos),
			vec3(this->vertices[indices[i + 2]].pos)
		};
		
		calc_intercept_vert(v, pos, path, path_n, normal_last, l);

		if(l == 0)
		{
			return path;
		}
	}

	return path;
}

mesh mesh::to_lines() const
{
	mesh m;
	m.vertices = vertices;

	for(int i = 0; i < indices.size(); i += 3)
	{
		m.indices.push_back(indices[i]);
		m.indices.push_back(indices[i + 1]);
		m.indices.push_back(indices[i + 1]);
		m.indices.push_back(indices[i + 2]);
		m.indices.push_back(indices[i]);
		m.indices.push_back(indices[i + 2]);
	}

	return m;
}

