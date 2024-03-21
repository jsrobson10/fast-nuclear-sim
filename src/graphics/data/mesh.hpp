
#pragma once

#include <string>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>

#include <glm/matrix.hpp>

#include "arrays.hpp"
#include "light.hpp"

namespace Sim::Graphics::Data
{

struct Mesh
{
	std::vector<Arrays::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::mat4> transforms;
	bool focus = false;

	Mesh();

	Mesh& bake_transforms();
	Mesh& set_blank_transform();
	Mesh& set_normal_id(unsigned int id);
	Mesh& set_diffuse_id(unsigned int id);
	Mesh& add(const Mesh& o, glm::mat4 mat = glm::mat4(1), bool bake = false);

	Mesh to_lines() const;
	bool check_focus(double len = 2.5) const;
	bool check_focus_hold(double len = 2.5);
	bool check_intersect(glm::vec<3, double> pos, glm::vec<3, double> path) const;
	glm::vec<3, double> calc_intersect(glm::vec<3, double> pos, glm::vec<3, double> path) const;

	bool operator==(const Mesh&) const = default;

	friend std::ostream& operator<<(std::ostream& os, const Mesh& m);
};

};

