
#pragma once

#include "../data/meshgen.hpp"

namespace Sim::Graphics::Equipment
{

class PoolPump : public Data::MeshGen
{
	Data::Mesh g_light_load;
	Data::Mesh g_light_unload;
	Data::Mesh g_light_idle;

public:

	PoolPump(const Data::Model& model);

	void update(double dt) override;
	void remesh_static(Data::Mesh& rmesh) override;
	void get_static_materials(std::vector<Data::Material>& materials) override;
};

};

