
#pragma once

#include "mesh.hpp"

#include <vector>
#include <string>

namespace sim::graphics
{

struct model
{
	std::vector<mesh> meshes;

	void load(std::string base, std::string path);
	void render() const;
};

};

