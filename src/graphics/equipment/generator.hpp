
#pragma once

#include "../data/meshgen.hpp"
#include "../../audio/source.hpp"

namespace Sim::Graphics::Equipment
{

class Generator : public Data::MeshGen
{
	Data::Mesh g_rotor;
//	Audio::Source g_sound_turbine;
	Audio::Source g_sound_generator;

public:

	Generator(const Data::Model& model);
	void get_static_transforms(std::vector<glm::mat4>& transforms) override;
	void remesh_static(Data::Mesh& rmesh) override;
	void update(double dt) override;
};

};

