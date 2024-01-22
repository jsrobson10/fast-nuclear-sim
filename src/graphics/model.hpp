
#pragma once

#include "mesh.hpp"

#include <vector>

namespace sim::graphics
{

struct model
{
	std::vector<mesh> meshes;

	void load(const char* path);
	void render() const;
};

};

