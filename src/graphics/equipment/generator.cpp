
#include "generator.hpp"
#include "../../system.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace Sim::Graphics::Equipment;

Generator::Generator(const Model& model)
{
	g_rotor = model.load("visual_generator_rotor");
}

void Generator::remesh_static(Mesh& rmesh)
{
	rmesh.add(g_rotor);
}

void Generator::get_static_transforms(std::vector<glm::mat4>& transforms)
{
	Sim::System& sys = *Sim::System::active;
	glm::mat4 rot = glm::rotate(glm::mat4(1), (float)sys.loop.generator.get_phase(), glm::vec3(0, 0, 1));
	transforms.push_back(rot);
}

