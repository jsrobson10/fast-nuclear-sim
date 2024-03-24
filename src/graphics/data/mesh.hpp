
#pragma once

#include <string>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>

#include <glm/matrix.hpp>

#include "../input/focus.hpp"
#include "arrays.hpp"
#include "light.hpp"

namespace Sim::Graphics::Data
{

struct Mesh
{
	struct PrimitiveItem
	{
		glm::vec2 texpos = {0, 0};
		glm::vec3 pos = {0, 0, 0};
	};

	template <int N_VERTS, int N_INDICES>
	struct Primitive
	{
		Arrays::Vertex vertex_base;
		unsigned int indices[N_INDICES];
		PrimitiveItem vertices[N_VERTS];
	};

	int material_ids = 0;
	std::vector<Arrays::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::mat4> transforms;
	bool focus = false;
	bool baked = false;

	Mesh(bool baked = false);

	void set_baked(bool b = true);
	void set_material_id(int id = 0);
	void set_blank_transform();
	void set_normal_id(unsigned int id);
	void set_diffuse_id(unsigned int id);
	void add(const Mesh& o, glm::mat4 mat = glm::mat4(1), bool bake = false);
	void clear();

	Mesh to_lines() const;
	bool check_focus(Focus::Trigger level = Focus::Trigger::NONE, double len = 2.5) const;
	bool check_focus_hold(Focus::Trigger level = Focus::Trigger::NONE, double len = 2.5);
	bool check_intersect(glm::vec<3, double> pos, glm::vec<3, double> path) const;
	glm::vec<3, double> calc_intersect(glm::vec<3, double> pos, glm::vec<3, double> path) const;

	bool operator==(const Mesh&) const = default;

	friend std::ostream& operator<<(std::ostream& os, const Mesh& m);

	template <int N_VERTS, int N_INDICES>
	void add(const Primitive<N_VERTS, N_INDICES>& prim, glm::mat4 mat = glm::mat4(1), bool bake = false)
	{
		int transform_id = -1;
		int off = vertices.size();
		Arrays::Vertex v = prim.vertex_base;

		if(!bake && !baked)
		{
			transform_id = transforms.size();
			transforms.push_back(mat);
			mat = glm::mat4(1);
		}

		for(int i = 0; i < N_VERTS; i++)
		{
			v.transform_id = transform_id;
			v.pos = glm::vec3(mat * glm::vec4(prim.vertices[i].pos, 1));
			v.tbn = glm::mat3(mat) * prim.vertex_base.tbn;
			v.texpos = prim.vertices[i].texpos;
			vertices.push_back(v);
		}

		for(int i = 0; i < N_INDICES; i++)
		{
			indices.push_back(prim.indices[i] + off);
		}
	}
};

};

