
#include <AL/alut.h>

#include "generator.hpp"
#include "../../system.hpp"
#include "../../util/math.hpp"
#include "../camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

using namespace Sim::Graphics::Equipment;
using namespace Sim::Graphics::Data;

Generator::Generator(const Model& model)
	: MeshGen("generator")
	, g_sound_generator("generator.ogg")
{
	g_rotor = model.load("visual_generator_rotor");

	g_sound_generator.mat = model.load_matrix("audio_generator");
	g_sound_generator.gain = 0;
	g_sound_generator.process();
	g_sound_generator.loop();
}

void Generator::update(double dt)
{
	if(Camera::is_in_main_menu())
	{
		g_sound_generator.gain = 0;
		return;
	}

	Sim::System& sys = *Sim::System::active;
	g_sound_generator.pitch = Util::Math::map(sys.loop.generator.get_rpm(), 0, 1800, 0, 1);
	g_sound_generator.gain = 1;
}

void Generator::remesh_static(Mesh& rmesh)
{
	rmesh.add(g_rotor);
}

void Generator::get_static_transforms(std::vector<glm::mat4>& transforms)
{
	Sim::System& sys = *Sim::System::active;
	glm::mat4 rot = glm::rotate(glm::mat4(1), (float)sys.loop.generator.get_phase() * 0.5f, glm::vec3(0, 0, 1));
	transforms.push_back(rot);
}

