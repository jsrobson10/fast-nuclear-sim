
#pragma once

#include <string>
#include <sstream>
#include <vector>

#include <glm/matrix.hpp>

#include "arrays.hpp"


namespace Sim::Graphics
{

struct Mesh
{
	std::vector<Arrays::Vertex> vertices;
	std::vector<unsigned int> indices;

	constexpr Mesh() { }

	void set_vertices(const Arrays::Vertex* data, size_t size);
	void set_indices(const unsigned int* data, size_t size);
	void load_model(std::string base, std::string path);
	void load_model(std::string path) { load_model(".", path); }
	void load_text(const char* text, double size);
	void add(const Mesh& o, glm::mat4 mat);
	void add(const Mesh& o) { add(o, glm::mat4(1)); }

	Mesh to_lines() const;
	bool check_focus() const;
	bool check_focus(double len) const;
	bool check_intersect(glm::vec<3, double> pos, glm::vec<3, double> path) const;
	glm::vec<3, double> calc_intersect(glm::vec<3, double> pos, glm::vec<3, double> path) const;
	glm::vec<3, double> calc_intersect(glm::vec<3, double> pos, glm::vec<3, double> path, glm::vec<3, double>& normal_last) const;

	template <class T>
	void load_text(const char* header, T& item, double size)
	{
		std::stringstream ss;
		ss << header << item;
		load_text(ss.str().c_str(), size);
	}
};

};

