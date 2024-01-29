
#pragma once

#include <string>
#include <sstream>
#include <vector>

#include <glm/matrix.hpp>

#include "arrays.hpp"


namespace sim::graphics
{

struct mesh
{
	std::vector<arrays::vertex> vertices;
	std::vector<unsigned int> indices;

	constexpr mesh() { }

	void set_vertices(const arrays::vertex* data, size_t size);
	void set_indices(const unsigned int* data, size_t size);
	void load_model(std::string base, std::string path);
	void load_model(std::string path) { load_model(".", path); }
	void load_text(const char* text, double size);
	void add(const mesh& o, glm::mat4 mat);

	glm::vec<3, double> check_intersect(glm::vec<3, double> pos, glm::vec<3, double> path) const;

	template <class T>
	void load_text(const char* header, T& item, double size)
	{
		std::stringstream ss;
		ss << header << item;
		load_text(ss.str().c_str(), size);
	}
};

};

