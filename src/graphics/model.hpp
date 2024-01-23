
#pragma once

#include "mesh.hpp"

#include <glm/vec3.hpp>
#include <vector>
#include <string>

namespace sim::graphics
{

struct model
{
	std::vector<mesh> meshes;

	void load(std::string base, std::string path, glm::vec<3, double> offset);
	void load(std::string base, std::string path) { load(base, path, {0, 0, 0}); }
	void render() const;
};

};

