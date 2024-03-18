
#include "reactor.hpp"
#include "../../system.hpp"
#include "../../reactor/rod.hpp"
#include "../../reactor/coolant/vessel.hpp"

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Sim::Graphics::Equipment;
using namespace Sim::Graphics::Data;

Reactor::Reactor(const Model& model)
{
	g_control_rod_lift = model.load("visual_control_rod_lift");
	g_control_rod_base = model.load("visual_control_rod_base");
}

void Reactor::remesh_static(Mesh& rmesh)
{
	Sim::System& sys = *Sim::System::active;
	double t_step = sys.reactor.cell_width;
	double t_sx = -(sys.reactor.width - 1) * t_step / 2.0;
	double t_sy = -(sys.reactor.height - 1) * t_step / 2.0;

	glm::mat4 mat_scale = glm::scale(glm::mat4(1), glm::vec3(t_step / 0.4));

	for(int i = 0; i < sys.reactor.size; i++)
	{
		int x = i % sys.reactor.width;
		int y = i / sys.reactor.width;
		double ox = t_sx + x * t_step;
		double oy = t_sy + y * t_step;
		
		Sim::Reactor::Rod* r = sys.reactor.rods[i].get();

		if(!r->should_display())
		{
			continue;
		}

		if(r->get_colour()[3] != 0)
		{
			Mesh m1, m2;
			glm::mat4 m = glm::translate(glm::mat4(1), glm::vec3(ox, oy, 0)) * mat_scale;
			m1.add(g_control_rod_base, m);
			m2.add(g_control_rod_lift, m);
			m1.bake_transforms();
			m2.bake_transforms();
			m2.set_blank_transform();

			for(int i = 0; i < m2.vertices.size(); i++)
			{
				if(g_control_rod_lift.vertices[i].pos.z == 0)
				{
					m2.vertices[i].transform_id = -1;
				}
			}

			rmesh.add(m1);
			rmesh.add(m2);
		}
	}
}

void Reactor::get_static_transforms(std::vector<glm::mat4>& transforms)
{
	Sim::System& sys = *Sim::System::active;

	for(int i = 0; i < sys.reactor.size; i++)
	{
		Sim::Reactor::Rod* r = sys.reactor.rods[i].get();

		if(!r->should_display())
		{
			continue;
		}

		if(r->get_colour()[3] != 0)
		{
			transforms.push_back(glm::translate(glm::mat4(1), glm::vec3(0, 0, (1 - r->get_colour().r) * sys.reactor.cell_height)));
		}
	}
}

