
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "primary_loop.hpp"
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

struct ValveJoystick : public Focus::FocusType
{
	Sim::Coolant::Valve* active;
	
	ValveJoystick(Sim::Coolant::Valve* v) : active(v)
	{
		cursor_is_visible = false;
	}

	virtual ~ValveJoystick()
	{
		active->clear_open_speed();
	}
	
	virtual void on_cursor_pos(double x, double y)
	{
		active->add_open_speed(-y * 1e-6);
	}

	virtual void on_mouse_button(int button, int action, int mods)
	{
		if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			Focus::clear_focus();
		}
	}
};

PrimaryLoop::PrimaryLoop(const Model& model)
	: Data::MeshGen("primary loop")
	, a_click(model, {"visual_pump_switch_1", "visual_inlet_switch", "visual_bypass_switch"}, {"click_1.ogg", "click_2.ogg"})
{
	mat = model.load_matrix("translation_monitor_4");
	
	g_switch_pump = model.load("visual_pump_switch_1");
	g_switch_bypass = model.load("visual_bypass_switch");
	g_switch_inlet = model.load("visual_inlet_switch");

	m_joystick_turbine_bypass = model.load("click_bypass_joystick");
	m_joystick_turbine_inlet = model.load("click_inlet_joystick");
	m_switch_pump = model.load("click_pump_switch_1");
	m_switch_bypass = model.load("click_bypass_switch");
	m_switch_inlet = model.load("click_inlet_switch");
}

void PrimaryLoop::remesh_static(Mesh& rmesh)
{
	std::stringstream ss;

	ss << "Turbine Bypass Valve\n\n";
	ss << "Opened\nFlow\nSetpoint\n\n";
	ss << "Turbine Inlet Valve\n\n";
	ss << "Opened\nFlow\nSetpoint\n\n";
	ss << "Primary Pump\n\n";
	ss << "Power\nSpeed\nFlow\n\n";
	ss << "Condenser\n\n";
	ss << "Heat\n";
	ss << "Steam\n";
	ss << "Pressure\n";
	ss << "Level\n";

	Data::Fonts::BASE.load_text(rmesh, ss.str(), {.size=0.04, .mat=mat, .bake=true});
	rmesh.add(g_switch_pump);
	rmesh.add(g_switch_bypass);
	rmesh.add(g_switch_inlet);
}

void PrimaryLoop::update(double dt)
{
	System& sys = *System::active;

	if(m_joystick_turbine_bypass.check_focus(Focus::Trigger::INTERFACE))
		Focus::set(std::make_unique<ValveJoystick>(&sys.loop.turbine_bypass_valve));
	if(m_joystick_turbine_inlet.check_focus(Focus::Trigger::INTERFACE))
		Focus::set(std::make_unique<ValveJoystick>(&sys.loop.turbine_inlet_valve));
	if(m_switch_pump.check_focus(Focus::Trigger::INTERFACE)) {
		sys.loop.primary_pump.powered = !sys.loop.primary_pump.powered;
		a_click.play(0);
	}
	if(m_switch_inlet.check_focus(Focus::Trigger::INTERFACE)) {
		sys.loop.turbine_inlet_valve.toggle_auto();
		a_click.play(1);
	}
	if(m_switch_bypass.check_focus(Focus::Trigger::INTERFACE)) {
		sys.loop.turbine_bypass_valve.toggle_auto();
		a_click.play(2);
	}
}

void PrimaryLoop::remesh_slow(Mesh& rmesh)
{
	std::stringstream ss;
	System& sys = *System::active;

	ss << "\n\n";
	ss << show( sys.loop.turbine_bypass_valve.get_state() * 100 ) << " %\n";
	show_units( ss, sys.loop.turbine_bypass_valve.get_flow() ) << "g/s\n";

	if(sys.loop.turbine_bypass_valve.get_auto())
	{
		ss << show( sys.loop.turbine_bypass_valve.get_setpoint() ) << " C\n";
	}

	else
	{
		ss << "-\n";
	}

	ss << "\n\n\n";
	ss << show( sys.loop.turbine_inlet_valve.get_state() * 100 ) << " %\n";
	show_units( ss, sys.loop.turbine_inlet_valve.get_flow() ) << "g/s\n";

	if(sys.loop.turbine_inlet_valve.get_auto())
	{
		ss << show( sys.loop.turbine_inlet_valve.get_setpoint() ) << " C\n";
	}

	else
	{
		ss << "-\n";
	}

	ss << "\n\n\n";
	ss << show( sys.loop.primary_pump.get_power() * 100 ) << " %\n";
	ss << show( sys.loop.primary_pump.get_rpm() ) << " r/min\n";
	show_units( ss, sys.loop.primary_pump.get_flow_mass() ) << "g/s\n";
	ss << "\n\n\n";
	ss << show( sys.loop.condenser.get_heat() ) << " C\n";
	show_units( ss, sys.loop.condenser.get_steam() ) << "g\n";
	show_units( ss, sys.loop.condenser.get_pressure() ) << "Pa\n";
	ss << show( sys.loop.condenser.get_level() / 1000 ) << " / " << show( sys.loop.condenser.get_volume() / 1000 ) << " kL\n";

	Data::Fonts::BASE.load_text(rmesh, ss.str(), {.size=0.04, .mat=glm::translate(mat, {0.5, 0, 0}), .bake=true});
}

void PrimaryLoop::get_static_transforms(std::vector<glm::mat4>& transforms)
{
	System& sys = *System::active;

	float off1 = sys.loop.primary_pump.powered ? 0.07 : 0;
	float off2 = sys.loop.turbine_bypass_valve.get_auto() ? 0.07 : 0;
	float off3 = sys.loop.turbine_inlet_valve.get_auto() ? 0.07 : 0;

	transforms.push_back(glm::translate(glm::mat4(1), glm::vec3(0, off1, 0)));
	transforms.push_back(glm::translate(glm::mat4(1), glm::vec3(0, off2, 0)));
	transforms.push_back(glm::translate(glm::mat4(1), glm::vec3(0, off3, 0)));
}

