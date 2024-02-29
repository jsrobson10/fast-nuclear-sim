
#pragma once

#include "../mesh/model.hpp"
#include "../mesh/glmesh.hpp"

namespace Sim::Graphics::Monitor
{

class Turbine
{
	glm::mat4 mat;

	GLMesh gm_synchroscope_dial;
	Mesh g_switch_breaker;
	Mesh m_switch_breaker;

public:

	Turbine();
	void init(const Model& model, Mesh& rmesh);
	void update(double dt);
	void remesh_slow(Mesh& rmesh);
	void remesh_fast(Mesh& rmesh);
	void render();
};

};

