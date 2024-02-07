
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "helpers.hpp"
#include "primary_loop.hpp"
#include "../locations.hpp"
#include "../../system.hpp"
#include "../../coolant/valve.hpp"
#include "../input/focus.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <iostream>

using namespace sim::graphics;
using namespace sim::graphics::monitor;

struct valve_joystick : public focus::focus_t
{
	sim::coolant::valve* active;
	
	valve_joystick(sim::coolant::valve* v) : active(v)
	{
		
	}

	virtual ~valve_joystick()
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
			focus::clear_focus();
		}
	}

	virtual bool cursor_is_visible()
	{
		return false;
	}
};


primary_loop::primary_loop()
{

}

void primary_loop::toggle_primary_pump()
{
	system& sys = sim::system::active;
	bool state;
	
	sys.primary_pump->powered = state = !sys.primary_pump->powered;
	gm_switch_1.model_matrix = glm::translate(glm::mat4(1), glm::vec3(0, state ? 0.07 : 0, 0));
}

void primary_loop::init()
{
	mesh1.model_matrix = locations::monitors[3];
	mesh2.model_matrix = glm::translate(mesh1.model_matrix, glm::vec3(0.5, 0, 0));

	mesh1.colour_matrix = mesh2.colour_matrix = {
		1, 1, 1, 1,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	
	std::stringstream ss;
	sim::graphics::mesh rmesh;

	ss << "Turbine Bypass Valve\n\n";
	ss << "Opened\nFlow\n\n";
	ss << "Turbine Inlet Valve\n\n";
	ss << "Opened\nFlow\n\n";
	ss << "Primary Pump\n\n";
	ss << "Power\nSpeed\nFlow\n\n";
	ss << "Condenser\n\n";
	ss << "Heat\n";
	ss << "Steam\n";
	ss << "Pressure\n";
	ss << "Level\n";

	rmesh.load_text(ss.str().c_str(), 0.04);
	mesh1.bind();
	mesh1.set(rmesh, GL_STATIC_DRAW);

	rmesh.load_model("../assets/model", "pump_switch_1.glb");
	gm_switch_1.bind();
	gm_switch_1.set(rmesh, GL_STATIC_DRAW);

	m_joystick_turbine_bypass.load_model("../assets/model", "turbine_valve_bypass_joystick.stl");
	m_joystick_turbine_inlet.load_model("../assets/model", "turbine_valve_inlet_joystick.stl");
	m_switch_1.load_model("../assets/model", "pump_switch_click_1.stl");
}

void primary_loop::update(double dt)
{
	std::stringstream ss;
	sim::graphics::mesh rmesh;
	system& sys = sim::system::active;

	ss << "\n\n";
	ss << show( sys.turbine_bypass_valve->get_state() * 100 ) << " %\n";
	ss << show( sys.turbine_bypass_valve->get_flow() / 1000 ) << " kg/s\n";
	ss << "\n\n\n";
	ss << show( sys.turbine_inlet_valve->get_state() * 100 ) << " %\n";
	ss << show( sys.turbine_inlet_valve->get_flow() / 1000 ) << " kg/s\n";
	ss << "\n\n\n";
	ss << show( sys.primary_pump->get_power() * 100 ) << " %\n";
	ss << show( sys.primary_pump->get_rpm() ) << " r/min\n";
	ss << show( sys.primary_pump->get_flow_mass() / 1000 ) << " kg/s\n";
	ss << "\n\n\n";
	ss << show( sys.condenser->get_heat() ) << " C\n";
	ss << show( sys.condenser->get_steam() ) << " g\n";
	ss << show( sys.condenser->get_pressure() / 1000 ) << " kPa\n";
	ss << show( sys.condenser->get_level() / 1000 ) << " / " << show( sys.condenser->get_volume() / 1000 ) << " kL\n";

	rmesh.load_text(ss.str().c_str(), 0.04);
	mesh2.bind();
	mesh2.set(rmesh, GL_DYNAMIC_DRAW);

	if(m_joystick_turbine_bypass.check_focus())
		focus::set(std::make_unique<valve_joystick>(sys.turbine_bypass_valve.get()));
	if(m_joystick_turbine_inlet.check_focus())
		focus::set(std::make_unique<valve_joystick>(sys.turbine_inlet_valve.get()));
	if(m_switch_1.check_focus())
		toggle_primary_pump();
}

void primary_loop::render()
{
	mesh1.bind();
	mesh1.uniform();
	mesh1.render();

	mesh2.bind();
	mesh2.uniform();
	mesh2.render();
	
	gm_switch_1.bind();
	gm_switch_1.uniform();
	gm_switch_1.render();
}

