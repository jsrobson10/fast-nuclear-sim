
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "secondary_loop.hpp"
#include "../locations.hpp"
#include "../../system.hpp"
#include "../../coolant/valve.hpp"
#include "../input/focus.hpp"
#include "../../util/streams.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <iostream>

using namespace Sim::Graphics;
using namespace Sim::Graphics::Monitor;
using namespace Sim::Util::Streams;

SecondaryLoop::SecondaryLoop()
{

}

void SecondaryLoop::init()
{
	mesh1.model_matrix = Locations::monitors[5];
	mesh2.model_matrix = glm::translate(mesh1.model_matrix, glm::vec3(0.5, 0, 0));

	mesh1.colour_matrix = mesh2.colour_matrix = {
		1, 1, 1, 1,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	
	std::stringstream ss;
	Sim::Graphics::Mesh rmesh;

	ss << "Cooling Tower\n\n";
	ss << "Heat\nSteam\nPressure\nLevel\n\n";
	ss << "Secondary Pump\n\n";
	ss << "Power\nSpeed\nFlow\n\n";
	ss << "Freight Pump\n\n";
	ss << "Power\nSpeed\nFlow\n\n";

	rmesh.load_text(ss.str().c_str(), 0.04);
	mesh1.bind();
	mesh1.set(rmesh, GL_STATIC_DRAW);

	rmesh.load_model("../assets/model", "pump_switch_2.fbx");
	gm_switch_2.bind();
	gm_switch_2.set(rmesh, GL_STATIC_DRAW);
	
	rmesh.load_model("../assets/model", "pump_switch_3.fbx");
	gm_switch_3.bind();
	gm_switch_3.set(rmesh, GL_STATIC_DRAW);

	m_joystick_turbine_bypass.load_model("../assets/model", "turbine_valve_bypass_joystick.stl");
	m_joystick_turbine_inlet.load_model("../assets/model", "turbine_valve_inlet_joystick.stl");
	m_switch_2.load_model("../assets/model", "pump_switch_click_2.stl");
	m_switch_3.load_model("../assets/model", "pump_switch_click_3.stl");
}

void SecondaryLoop::update(double dt)
{
	System& sys = Sim::System::active;
	clock_now += dt;

	if(clock_at + 1.0/30.0 < clock_now)
	{
		std::stringstream ss;
		Sim::Graphics::Mesh rmesh;
		clock_at += 1.0/30.0;

		ss << "\n\n";
		ss << show( sys.evaporator->get_heat() ) << " C\n";
		show_units( ss, sys.evaporator->get_steam_output() ) << "g/s\n";
		show_units( ss, sys.evaporator->get_pressure() ) << "Pa\n";
		ss << show( sys.evaporator->get_level() / 1000 ) << " / " << show( sys.evaporator->get_volume() / 1000 ) << " kL\n";
		ss << "\n\n\n";
		ss << show( sys.secondary_pump->get_power() * 100 ) << " %\n";
		ss << show( sys.secondary_pump->get_rpm() ) << " r/min\n";
		show_units( ss, sys.secondary_pump->get_flow_mass() ) << "g/s\n";
		ss << "\n\n\n";
		ss << show( sys.freight_pump->get_power() * 100 ) << " %\n";
		ss << show( sys.freight_pump->get_rpm() ) << " r/min\n";
		show_units( ss, sys.freight_pump->get_flow_mass() ) << "g/s\n";

		rmesh.load_text(ss.str().c_str(), 0.04);
		mesh2.bind();
		mesh2.set(rmesh, GL_DYNAMIC_DRAW);
	}

	if(m_switch_2.check_focus())
		sys.secondary_pump->powered = !sys.secondary_pump->powered;
	if(m_switch_3.check_focus())
		sys.freight_pump->powered = !sys.freight_pump->powered;
	
	gm_switch_2.model_matrix = glm::translate(glm::mat4(1), glm::vec3(0, sys.secondary_pump->powered ? 0.07 : 0, 0));
	gm_switch_3.model_matrix = glm::translate(glm::mat4(1), glm::vec3(0, sys.freight_pump->powered ? 0.07 : 0, 0));
}

void SecondaryLoop::render()
{
	mesh1.bind();
	mesh1.uniform();
	mesh1.render();

	mesh2.bind();
	mesh2.uniform();
	mesh2.render();
	
	gm_switch_2.bind();
	gm_switch_2.uniform();
	gm_switch_2.render();

	gm_switch_3.bind();
	gm_switch_3.uniform();
	gm_switch_3.render();
}


