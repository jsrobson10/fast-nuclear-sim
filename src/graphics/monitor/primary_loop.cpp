
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "primary_loop.hpp"
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

struct ValveJoystick : public Focus::FocusType
{
	Sim::Coolant::Valve* active;
	
	ValveJoystick(Sim::Coolant::Valve* v) : active(v)
	{
		
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

	virtual bool cursor_is_visible()
	{
		return false;
	}
};


PrimaryLoop::PrimaryLoop()
{

}

void PrimaryLoop::init(const Model& model, Mesh& rmesh)
{
	mat = Locations::monitors[3];
	
	std::stringstream ss;
	Sim::Graphics::Mesh mesh;

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

	mesh.load_text(ss.str().c_str(), 0.04);
	rmesh.add(mesh, mat);

	g_switch_pump = model.load("visual_pump_switch_1");
	g_switch_bypass = model.load("visual_bypass_switch");
	g_switch_inlet = model.load("visual_inlet_switch");

	m_joystick_turbine_bypass = model.load("click_bypass_joystick");
	m_joystick_turbine_inlet = model.load("click_inlet_joystick");
	m_switch_pump = model.load("click_pump_switch_1");
	m_switch_bypass = model.load("click_bypass_switch");
	m_switch_inlet = model.load("click_inlet_switch");
}

void PrimaryLoop::update(double dt)
{
	System& sys = *System::active;

	if(m_joystick_turbine_bypass.check_focus())
		Focus::set(std::make_unique<ValveJoystick>(&sys.loop.turbine_bypass_valve));
	if(m_joystick_turbine_inlet.check_focus())
		Focus::set(std::make_unique<ValveJoystick>(&sys.loop.turbine_inlet_valve));
	if(m_switch_pump.check_focus())
		sys.loop.primary_pump.powered = !sys.loop.primary_pump.powered;
	if(m_switch_inlet.check_focus())
		sys.loop.turbine_inlet_valve.toggle_auto();
	if(m_switch_bypass.check_focus())
		sys.loop.turbine_bypass_valve.toggle_auto();
}

void PrimaryLoop::remesh_slow(Mesh& rmesh)
{
	std::stringstream ss;
	Sim::Graphics::Mesh mesh;
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

	mesh.load_text(ss.str().c_str(), 0.04);
	rmesh.add(mesh, glm::translate(mat, glm::vec3(0.5, 0, 0)));
}

void PrimaryLoop::remesh_fast(Mesh& rmesh)
{
	System& sys = *System::active;

	float off1 = sys.loop.primary_pump.powered ? 0.07 : 0;
	float off2 = sys.loop.turbine_bypass_valve.get_auto() ? 0.07 : 0;
	float off3 = sys.loop.turbine_inlet_valve.get_auto() ? 0.07 : 0;

	rmesh.add(g_switch_pump, glm::translate(glm::mat4(1), glm::vec3(0, off1, 0)));
	rmesh.add(g_switch_bypass, glm::translate(glm::mat4(1), glm::vec3(0, off2, 0)));
	rmesh.add(g_switch_inlet, glm::translate(glm::mat4(1), glm::vec3(0, off3, 0)));
}

void PrimaryLoop::render()
{
}

