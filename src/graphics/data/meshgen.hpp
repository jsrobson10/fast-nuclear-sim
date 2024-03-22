
#pragma once

#include "model.hpp"

namespace Sim::Graphics::Data
{

class MeshGen
{
public:

	const char* name;

	constexpr MeshGen(const char* name) : name(name) {}

	virtual ~MeshGen() {}
	virtual void update(double dt) {};
	virtual void get_static_transforms(std::vector<glm::mat4>& transforms) {};
	virtual void get_static_colours(std::vector<glm::vec4>& colours) {};
	virtual void remesh_static(Mesh& rmesh) {};
	virtual void remesh_slow(Mesh& rmesh) {};
};

};

