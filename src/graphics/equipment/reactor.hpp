
#pragma once

#include "../mesh/meshgen.hpp"

namespace Sim::Graphics::Equipment
{

class Reactor : public MeshGen
{
	Mesh g_control_rod;

public:

	Reactor(const Model& model, Mesh& rmesh);
	virtual void update(double dt);
	virtual void remesh_slow(Mesh& rmesh);
	virtual void remesh_fast(Mesh& rmesh);
	virtual void render();
};

};

