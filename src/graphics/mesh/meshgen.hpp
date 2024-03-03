
#pragma once

#include "../mesh/model.hpp"

namespace Sim::Graphics
{

class MeshGen
{
public:
	virtual ~MeshGen() {}
	virtual void update(double dt) {};
	virtual void get_static_transforms(std::vector<glm::mat4>& transforms) {};
	virtual void remesh_static(Mesh& rmesh) {};
	virtual void remesh_slow(Mesh& rmesh) {};
};

};

