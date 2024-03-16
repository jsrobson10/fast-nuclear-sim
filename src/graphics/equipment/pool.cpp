
#include "pool.hpp"
#include "../../system.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace Sim::Graphics::Equipment;

Pool::Pool(const Model& model)
{
	g_pool = model.load("visual_water");
}

void Pool::remesh_static(Mesh& rmesh)
{
	rmesh.add(g_pool);
}

void Pool::get_static_transforms(std::vector<glm::mat4>& transforms)
{
	Sim::System& sys = *Sim::System::active;
	double z = sys.pool.get_level_height();

	if(z > 0.1)
	{
		transforms.push_back(glm::translate(glm::mat4(1), glm::vec3(0, 0, z - sys.pool.dimensions.z)));
	}

	else
	{
		// hacky solution to prevent z fighting with the bottom of the pool
		transforms.push_back(glm::mat4(0));
	}
}

