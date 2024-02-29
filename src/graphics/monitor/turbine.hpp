
#pragma once

#include "../mesh/model.hpp"
#include "../mesh/glmesh.hpp"
#include "../mesh/meshgen.hpp"

namespace Sim::Graphics::Monitor
{

class Turbine : public MeshGen
{
	glm::mat4 mat;

	GLMesh gm_synchroscope_dial;
	Mesh g_switch_breaker;
	Mesh m_switch_breaker;

public:

	Turbine(const Model& model, Mesh& rmesh);
	virtual void update(double dt);
	virtual void remesh_slow(Mesh& rmesh);
	virtual void remesh_fast(Mesh& rmesh);
	virtual void render();
};

};

