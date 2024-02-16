
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "helpers.hpp"
#include "turbine.hpp"
#include "../locations.hpp"
#include "../../system.hpp"
#include "../../coolant/valve.hpp"
#include "../input/focus.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <iostream>

using namespace Sim::Graphics;
using namespace Sim::Graphics::Monitor;

Turbine::Turbine()
{

}

void Turbine::init()
{
	mesh1.model_matrix = mesh2.model_matrix = Locations::monitors[4];
	mesh1.colour_matrix = mesh2.colour_matrix = {
		1, 1, 1, 1,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	
	std::stringstream ss;
	Sim::Graphics::Mesh rmesh, rmesh2;

	ss << "Turbine\n\n";
	ss << "Heat\nPressure\nSpeed\n\n";

	rmesh.load_text(ss.str().c_str(), 0.04);
	rmesh2.load_text("Synchroscope", 0.04);
	rmesh.add(rmesh2, glm::translate(glm::mat4(1), glm::vec3(0, 0.6, 0)));

	mesh1.bind();
	mesh1.set(rmesh, GL_STATIC_DRAW);
	
	rmesh.load_model("../assets/model", "synchroscope_dial.stl");
	gm_synchroscope_dial.bind();
	gm_synchroscope_dial.set(rmesh, GL_STATIC_DRAW);

	rmesh.load_model("../assets/model", "turbine_breaker_switch.fbx");
	gm_switch_breaker.bind();
	gm_switch_breaker.set(rmesh, GL_STATIC_DRAW);

	m_switch_breaker.load_model("../assets/model", "turbine_breaker_switch_click.stl");
}

void Turbine::update(double dt)
{
	System& sys = Sim::System::active;
	clock_now += dt;

	if(clock_at + 1.0/30.0 < clock_now)
	{
		std::stringstream ss;
		Sim::Graphics::Mesh rmesh, rmesh2;
		clock_at += 1.0/30.0;

		ss << "\n\n";
		ss << show( sys.turbine->get_heat() ) << " C\n";
		ss << show( sys.turbine->get_pressure() / 1000 ) << " kPa\n";
		ss << show( sys.turbine->get_rpm() ) << " r/min\n";
		
		rmesh2.load_text(ss.str().c_str(), 0.04);
		rmesh.add(rmesh2, glm::translate(glm::mat4(1), glm::vec3(0.5, 0, 0)));

		ss = std::stringstream();

		ss << "Local\n\n";
		ss << show( sys.turbine->get_rpm() / 60 ) << " Hz\n";
		ss << show( sys.turbine->get_energy_generated() ) << " W\n";

		rmesh2.load_text(ss.str().c_str(), 0.04);
		rmesh.add(rmesh2, glm::translate(glm::mat4(1), glm::vec3(0.4, 0.7, 0)));

		ss = std::stringstream();

		ss << "Grid\n\n";
		ss << show( 60 ) << " Hz\n";

		rmesh2.load_text(ss.str().c_str(), 0.04);
		rmesh.add(rmesh2, glm::translate(glm::mat4(1), glm::vec3(0.7, 0.7, 0)));

		mesh2.bind();
		mesh2.set(rmesh, GL_DYNAMIC_DRAW);
	}

	double rpm = sys.turbine->get_rpm();

	if(rpm > 3570 && rpm < 3630)
	{
		glm::mat4 mat = glm::mat4(1);
		mat = glm::translate(mat, glm::vec3(6.35, 3.949, 1.35));
		mat = glm::rotate(mat, float(sys.turbine->get_phase_diff()), glm::vec3(0, 1, 0));
		mat = glm::translate(mat, glm::vec3(-6.35, -3.949, -1.35));
		gm_synchroscope_dial.model_matrix = mat;
	}

	if(m_switch_breaker.check_focus())
		sys.turbine->breaker_closed = !sys.turbine->breaker_closed;

	gm_switch_breaker.model_matrix = glm::translate(glm::mat4(1), glm::vec3(0, sys.turbine->breaker_closed ? 0.07 : 0, 0));
}

void Turbine::render()
{
	mesh1.bind();
	mesh1.uniform();
	mesh1.render();

	mesh2.bind();
	mesh2.uniform();
	mesh2.render();

	double rpm = System::active.turbine->get_rpm();

	if(rpm > 3570 && rpm < 3630)
	{
		gm_synchroscope_dial.bind();
		gm_synchroscope_dial.uniform();
		gm_synchroscope_dial.render();
	}

	gm_switch_breaker.bind();
	gm_switch_breaker.uniform();
	gm_switch_breaker.render();
}

