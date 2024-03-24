
#include "mesh.hpp"
#include "arrays.hpp"
#include "../shader.hpp"
#include "../camera.hpp"
#include "../input/focus.hpp"
#include "../../util/math.hpp"
#include "../../util/streams.hpp"

#include <iostream>

using namespace Sim::Graphics::Data;

Mesh::Mesh(bool baked) : baked(baked)
{

}

void Mesh::set_diffuse_id(unsigned int id)
{
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		vertices[i].tex_diffuse = id;
	}
}

void Mesh::set_normal_id(unsigned int id)
{
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		vertices[i].tex_normal = id;
	}
}

void Mesh::set_material_id(int id)
{
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		vertices[i].material_id = id;
	}
}

void Mesh::set_blank_transform()
{
	transforms = {glm::mat4(1)};

	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		vertices[i].transform_id = 0;
	}
}

void Mesh::clear()
{
	vertices.clear();
	indices.clear();
	transforms.clear();
}

void Mesh::add(const Mesh& o, glm::mat4 mat, bool bake)
{
	int off = vertices.size();
	bool mat_set = mat != glm::mat4(1);

	if(baked || bake)
	{
		for(int i = 0; i < o.vertices.size(); i++)
		{
			Arrays::Vertex v = o.vertices[i];
			int t_id = v.transform_id;
			glm::mat4 t_mat = mat;
			
			if(t_id >= 0)
			{
				t_mat = o.transforms[t_id] * t_mat;
				v.transform_id = -1;
			}

			if(v.material_id >= 0)
			{
				v.material_id += material_ids;
			}
			
			v.pos = t_mat * glm::vec4(v.pos, 1);
			v.tbn = glm::mat3(t_mat) * v.tbn;

			vertices.push_back(v);
		}

		for(int i = 0; i < o.indices.size(); i++)
		{
			indices.push_back(o.indices[i] + off);
		}

		return;
	}

	glm::mat3 mat3(mat);
	int t_off = transforms.size();
	int t_new = -1;

	if(mat_set)
	{
		t_new = transforms.size() + o.transforms.size();
		
		for(int i = 0; i < o.transforms.size(); i++)
		{
			transforms.push_back(o.transforms[i] * mat);
		}

		if(t_new >= 0)
		{
			transforms.push_back(mat);
		}
	}

	else
	{
		transforms.insert(transforms.end(), o.transforms.begin(), o.transforms.end());
	}

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

		if(v.material_id >= 0)
		{
			v.material_id += material_ids;
		}
		
		vertices.push_back(v);
	}

	for(int i = 0; i < o.indices.size(); i++)
	{
		indices.push_back(o.indices[i] + off);
	}

	material_ids += o.material_ids;
}

void Mesh::set_baked(bool b)
{
	if(baked == b)
	{
		return;
	}

	baked = b;

	if(!b || transforms.size() == 0)
	{
		return;
	}

	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		int id = vertices[i].transform_id;

		if(id >= 0)
		{
			glm::mat4 transform = transforms[id];
			vertices[i].pos = glm::vec3(transform * glm::vec4(vertices[i].pos, 1));
			vertices[i].tbn = glm::mat3(transform) * vertices[i].tbn;
			vertices[i].transform_id = -1;
		}
	}

	transforms.clear();
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

bool Mesh::check_focus(Focus::Trigger level, double len) const
{
	if(!Focus::is_triggered(level))
	{
		return false;
	}

	auto near = Focus::get_trigger_near();
	auto far = Focus::get_trigger_far();
	
	return check_intersect(near, glm::normalize(far - near) * len);
}

bool Mesh::check_focus_hold(Focus::Trigger level, double len)
{
	if(!Focus::is_triggered(level) && (!focus || Focus::is_triggered_release()))
	{
		return focus = false;
	}

	auto near = Focus::get_trigger_near();
	auto far = Focus::get_trigger_far();
	
	return focus = check_intersect(near, glm::normalize(far - near) * len);
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
			int t_id = verts[j].transform_id;
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
				int t_id = verts[j].transform_id;
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
	Mesh m(*this);
	m.indices.clear();

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

std::ostream& Sim::Graphics::Data::operator<<(std::ostream& os, const Mesh& m)
{
	os << "Mesh(\n";
	os << "  Vertices(\n";

	for(int i = 0; i < m.vertices.size(); i++)
	{
		os << "    " << m.vertices[i] << "\n";
	}

	os << "  )\n";

	for(int i = 0; i < m.indices.size(); i += 3)
	{
		os << "    " << m.indices[i] << " " << m.indices[i + 1] << " " << m.indices[i + 2] << "\n";
	}

	os << "  Transforms(\n";

	for(int i = 0; i < m.transforms.size(); i++)
	{
		os << "    " << m.transforms[i] << "\n";
	}

	os << "  )\n";
	os << ")\n";

	return os;
}

