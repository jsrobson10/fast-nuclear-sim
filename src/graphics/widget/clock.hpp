
#pragma once

#include <string>

#include "../data/glmesh.hpp"

namespace Sim::Graphics::Widget
{

struct Clock
{
	std::string text;

	double dt;
	int i = 0;
	
	void update(double dt);
	void remesh_ui(Data::Mesh& rmesh);
};

};

