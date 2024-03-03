
#pragma once

#include <string>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>

#include <glm/matrix.hpp>

#include "arrays.hpp"
#include "light.hpp"

namespace Sim::Graphics
{

struct Mesh
{
	std::vector<Arrays::Vertex> vertices;
	std::vector<unsigned int> indices;
	float max_transform_id = -1;

	Mesh();

	void set_transform_id();
	void set_vertices(const Arrays::Vertex* data, size_t size);
	void set_indices(const unsigned int* data, size_t size);
	void load_text(const char* text, double size);
	void load_text(const char* text, double size, glm::vec2 align);
	void add(const Mesh& o, glm::mat4 mat);
	void add(const Mesh& o);

	Mesh to_lines() const;
	bool check_focus() const;
	bool check_focus(double len) const;
	bool check_intersect(glm::vec<3, double> pos, glm::vec<3, double> path) const;
	glm::vec<3, double> calc_intersect(glm::vec<3, double> pos, glm::vec<3, double> path) const;

	bool operator==(const Mesh&) const = default;

	template <class T>
	void load_text(const char* header, T& item, double size)
	{
		std::stringstream ss;
		ss << header << item;
		load_text(ss.str().c_str(), size);
	}
};

};

