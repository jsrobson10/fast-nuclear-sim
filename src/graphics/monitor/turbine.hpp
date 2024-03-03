
#pragma once

#include "../mesh/model.hpp"
#include "../mesh/glmesh.hpp"
#include "../mesh/meshgen.hpp"

namespace Sim::Graphics::Monitor
{

class Turbine : public MeshGen
{
	glm::mat4 mat;

	Mesh g_synchroscope_dial;
	Mesh g_switch_breaker;
	Mesh m_switch_breaker;

public:

	Turbine(const Model& model);
	virtual void update(double dt);
	virtual void get_static_transforms(std::vector<glm::mat4>& transforms);
	virtual void remesh_static(Mesh& rmesh);
	virtual void remesh_slow(Mesh& rmesh);
	virtual void remesh_fast(Mesh& rmesh);
};

};

