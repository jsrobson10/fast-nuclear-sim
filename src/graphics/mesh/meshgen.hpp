
#pragma once

#include "../mesh/model.hpp"

namespace Sim::Graphics
{

class MeshGen
{
public:
	virtual ~MeshGen() {}
	virtual void update(double dt) = 0;
	virtual void remesh_slow(Mesh& rmesh) = 0;
	virtual void remesh_fast(Mesh& rmesh) = 0;
	virtual void render() = 0;
};

};

