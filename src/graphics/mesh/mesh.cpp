
#include "mesh.hpp"
#include "arrays.hpp"
#include "../shader.hpp"
#include "../camera.hpp"
#include "../input/focus.hpp"
#include "../../util/math.hpp"

#include <iostream>

using namespace Sim::Graphics;

Mesh::Mesh()
{

}

Mesh& Mesh::set_texture_id(unsigned int id)
{
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		vertices[i].texid = id;
	}

	return *this;
}

Mesh& Mesh::set_blank_transform()
{
	transforms = {glm::mat4(1)};

	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		vertices[i].transform_id = 0;
	}

	return *this;
}

Mesh& Mesh::add(const Mesh& o, glm::mat4 mat, bool bake)
{
	int off = vertices.size();
	
	indices.reserve(indices.size() + o.indices.size());
	vertices.reserve(vertices.size() + o.vertices.size());

	if(bake)
	{
		for(int i = 0; i < o.vertices.size(); i++)
		{
			Arrays::Vertex v = o.vertices[i];
			int t_id = (int)v.transform_id;
			glm::mat4 t_mat = t_id >= 0 ? transforms[t_id] : glm::mat4(1);
			t_mat = mat * t_mat;
			
			v.pos = t_mat * glm::vec4(v.pos, 1);
			v.normal = glm::normalize(glm::mat3(t_mat) * v.normal);
			v.transform_id = -1;
			vertices.push_back(v);
		}

		for(int i = 0; i < o.indices.size(); i++)
		{
			indices.push_back(o.indices[i] + off);
		}

		return *this;
	}

	glm::mat3 mat3(mat);
	float t_off = transforms.size();
	float t_new = -1;

	if(mat != glm::mat4(1))
	{
		t_new = transforms.size() + o.transforms.size();
	}

	transforms.reserve(transforms.size() + o.transforms.size() + (t_new >= 0 ? 1 : 0));

	for(int i = 0; i < o.vertices.size(); i++)
	{
		Arrays::Vertex v = o.vertices[i];
		
		if(v.transform_id >= 0)
		{
			v.transform_id += t_off;
		}

		else
		{
			v.transform_id = t_new;
		}
		
		vertices.push_back(v);
	}

	for(int i = 0; i < o.transforms.size(); i++)
	{
		transforms.push_back(o.transforms[i] * mat);
	}

	for(int i = 0; i < o.indices.size(); i++)
	{
		indices.push_back(o.indices[i] + off);
	}

	if(t_new >= 0)
	{
		transforms.push_back(mat);
	}

	return *this;
}

Mesh& Mesh::bake_transforms()
{
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		int id = (int)vertices[i].transform_id;

		if(id >= 0)
		{
			glm::mat4 transform = transforms[id];
			vertices[i].pos = glm::vec3(transform * glm::vec4(vertices[i].pos, 1));
			vertices[i].normal = glm::normalize(glm::mat3(transform) * vertices[i].normal);
			vertices[i].transform_id = -1;
		}
	}

	transforms.clear();

	return *this;
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

bool Mesh::check_focus(double len) const
{
	auto near = Focus::get_trigger_near();
	auto far = Focus::get_trigger_far();
	
	return Focus::is_triggered() && check_intersect(near, glm::normalize(far - near) * len);
}

bool Mesh::check_focus() const
{
	return check_focus(2.5);
}

bool Mesh::check_intersect(vec3 pos, vec3 path) const
{
	double l = glm::length(path);

	if(l == 0)
	{
		return false;
	}

	vec3 path_n = path / l;
	
	for(unsigned int i = 0; i < indices.size(); i += 3)
	{
		Arrays::Vertex verts[3] = {
			vertices[indices[i]],
			vertices[indices[i + 1]],
			vertices[indices[i + 2]]
		};

		vec3 v[3];

		for(int j = 0; j < 3; j++)
		{
			int t_id = (int)verts[j].transform_id;
			glm::mat4 t_mat = t_id >= 0 ? transforms[t_id] : glm::mat4(1);

			v[j] = vec3(t_mat * glm::vec4(verts[j].pos, 1));
		}
		
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

static bool calc_intercept_vert(vec3 v[3], vec3 pos, vec3& path, vec3& path_n, double& l)
{
	vec3 ipoint;
	vec3 normal = glm::normalize(glm::cross(v[1] - v[0], v[2] - v[0]));
	double d = glm::dot(path, normal);

	if(d >= 0)
		return false;
	if(!ray_intersects_triangle(pos, path_n, v, ipoint))
		return false;
	if(l < glm::length(ipoint - pos))
		return false;

	if(d < 0)
	{
		path -= normal * d;
		l = glm::length(path);
	}

	if(l > 0)
	{
		path_n = path / l;
	}

	return true;
}

vec3 Mesh::calc_intersect(vec3 pos, vec3 path) const
{
	double l = glm::length(path);

	if(l == 0)
	{
		return path;
	}

	vec3 path_n = path / l;
	bool changing = true;

	while(changing)
	{
		changing = false;

		for(unsigned int i = 0; i < indices.size(); i += 3)
		{
			Arrays::Vertex verts[3] = {
				vertices[indices[i]],
				vertices[indices[i + 1]],
				vertices[indices[i + 2]]
			};

			vec3 v[3];

			for(int j = 0; j < 3; j++)
			{
				int t_id = (int)verts[j].transform_id;
				glm::mat4 t_mat = t_id >= 0 ? transforms[t_id] : glm::mat4(1);

				v[j] = vec3(t_mat * glm::vec4(verts[j].pos, 1));
			}
			
			if(calc_intercept_vert(v, pos, path, path_n, l))
			{
				changing = true;
			}

			if(l == 0)
			{
				return path;
			}
		}
	}

	return path;
}

Mesh Mesh::to_lines() const
{
	Mesh m;
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

