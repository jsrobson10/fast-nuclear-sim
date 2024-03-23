
#include "pool_pump.hpp"

using namespace Sim::Graphics::Equipment;

PoolPump::PoolPump(const Data::Model& model) : MeshGen("pool_pump")
{
	g_light_load = model.load("visual_pool_pump_load");
	g_light_unload = model.load("visual_pool_pump_unload");
	g_light_idle = model.load("visual_pool_pump_idle");

	Data::Mesh* meshes[] = { &g_light_load, &g_light_unload, &g_light_idle };

	for (Data::Mesh* mesh : meshes)
	{
		mesh->set_baked();
		mesh->set_material_id();
		mesh->material_ids = 1;
	}
}

void PoolPump::remesh_static(Data::Mesh& rmesh)
{
	rmesh.add(g_light_load);
	rmesh.add(g_light_unload);
	rmesh.add(g_light_idle);
}

void PoolPump::get_static_materials(std::vector<Data::Material>& materials)
{
	for(int i = 0; i < 3; i++)
	{
		// TODO: implement this
		bool on = i == 2;

		materials.push_back({
			.colour = {glm::vec3(on ? 2.f : 0.25f), 1},
			.luminance = on ? 1.f : 0.f,
		});
	}
}

void PoolPump::update(double dt)
{

}

