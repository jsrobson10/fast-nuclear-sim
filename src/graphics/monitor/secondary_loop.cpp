
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "secondary_loop.hpp"
#include "../../system.hpp"
#include "../../coolant/valve.hpp"
#include "../input/focus.hpp"
#include "../../util/streams.hpp"
#include "../data/font.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <iostream>

using namespace Sim::Graphics;
using namespace Sim::Graphics::Monitor;
using namespace Sim::Graphics::Data;
using namespace Sim::Util::Streams;

SecondaryLoop::SecondaryLoop(const Model& model) : Data::MeshGen("secondary loop")
{
	mat = model.load_matrix("translation_monitor_6");

	g_switch_2 = model.load("visual_pump_switch_2");
	g_switch_3 = model.load("visual_pump_switch_3");

	m_joystick_turbine_bypass = model.load("click_bypass_joystick");
	m_joystick_turbine_inlet = model.load("click_inlet_joystick");
	m_switch_2 = model.load("click_pump_switch_2");
	m_switch_3 = model.load("click_pump_switch_3");
}

void SecondaryLoop::update(double dt)
{
	System& sys = *System::active;

	if(m_switch_2.check_focus())
		sys.loop.secondary_pump.powered = !sys.loop.secondary_pump.powered;
	if(m_switch_3.check_focus())
		sys.freight_pump.powered = !sys.freight_pump.powered;
}

void SecondaryLoop::remesh_static(Mesh& rmesh)
{
	std::stringstream ss;

	ss << "Cooling Tower\n\n";
	ss << "Heat\nSteam\nPressure\nLevel\n\n";
	ss << "Secondary Pump\n\n";
	ss << "Power\nSpeed\nFlow\n\n";
	ss << "Freight Pump\n\n";
	ss << "Power\nSpeed\nFlow\n\n";

	Data::Fonts::BASE.load_text(rmesh, ss.str(), {.size=0.04, .mat=mat, .bake=true});
	rmesh.add(g_switch_2);
	rmesh.add(g_switch_3);
}

void SecondaryLoop::remesh_slow(Mesh& rmesh)
{
	std::stringstream ss;
	System& sys = *System::active;

	ss << "\n\n";
	ss << show( sys.evaporator.get_heat() ) << " C\n";
	show_units( ss, sys.evaporator.get_steam_output() ) << "g/s\n";
	show_units( ss, sys.evaporator.get_pressure() ) << "Pa\n";
	ss << show( sys.evaporator.get_level() / 1000 ) << " / " << show( sys.evaporator.get_volume() / 1000 ) << " kL\n";
	ss << "\n\n\n";
	ss << show( sys.loop.secondary_pump.get_power() * 100 ) << " %\n";
	ss << show( sys.loop.secondary_pump.get_rpm() ) << " r/min\n";
	show_units( ss, sys.loop.secondary_pump.get_flow_mass() ) << "g/s\n";
	ss << "\n\n\n";
	ss << show( sys.freight_pump.get_power() * 100 ) << " %\n";
	ss << show( sys.freight_pump.get_rpm() ) << " r/min\n";
	show_units( ss, sys.freight_pump.get_flow_mass() ) << "g/s\n";

	Data::Fonts::BASE.load_text(rmesh, ss.str().c_str(), {.size=0.04, .mat=glm::translate(mat, {0.5, 0, 0}), .bake=true});
}

void SecondaryLoop::get_static_transforms(std::vector<glm::mat4>& transforms)
{
	System& sys = *System::active;

	float off2 = sys.loop.secondary_pump.powered ? 0.07 : 0;
	float off3 = sys.freight_pump.powered ? 0.07 : 0;

	transforms.push_back(glm::translate(glm::mat4(1), glm::vec3(0, off2, 0)));
	transforms.push_back(glm::translate(glm::mat4(1), glm::vec3(0, off3, 0)));
}

