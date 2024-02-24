
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "turbine.hpp"
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

Turbine::Turbine()
{

}

void Turbine::init(Mesh& rmesh)
{
	mat = Locations::monitors[4];
	
	std::stringstream ss;
	Sim::Graphics::Mesh mesh;

	ss << "Turbine\n\n";
	ss << "Heat\nPressure\nSpeed\n\n";

	mesh.load_text(ss.str().c_str(), 0.04);
	rmesh.add(mesh, mat);

	mesh.load_text("Synchroscope", 0.04);
	rmesh.add(mesh, glm::translate(mat, glm::vec3(0, 0.6, 0)));
	
	mesh.load_model("../assets/model", "synchroscope_dial.glb");
	gm_synchroscope_dial.bind();
	gm_synchroscope_dial.set(mesh, GL_STATIC_DRAW);

	g_switch_breaker.load_model("../assets/model", "turbine_breaker_switch.glb");
	m_switch_breaker.load_model("../assets/model", "turbine_breaker_switch_click.stl");
}

void Turbine::update(double dt)
{
	System& sys = *System::active;
	double rpm = sys.loop.generator.get_rpm();

	if(rpm > 3570 && rpm < 3630)
	{
		glm::mat4 mat = glm::mat4(1);
		mat = glm::translate(mat, glm::vec3(6.35, 3.949, 1.35));
		mat = glm::rotate(mat, float(sys.loop.generator.get_phase_diff()), glm::vec3(0, 1, 0));
		mat = glm::translate(mat, glm::vec3(-6.35, -3.949, -1.35));
		gm_synchroscope_dial.model_matrix = mat;
	}

	if(m_switch_breaker.check_focus())
		sys.loop.generator.breaker_closed = !sys.loop.generator.breaker_closed;

}

void Turbine::remesh_slow(Mesh& rmesh)
{
	std::stringstream ss;
	Sim::Graphics::Mesh mesh;
	System& sys = *System::active;

	ss << "\n\n";
	ss << show( sys.loop.turbine.get_heat() ) << " C\n";
	ss << show( sys.loop.turbine.get_pressure() / 1000 ) << " kPa\n";
	ss << show( sys.loop.generator.get_rpm() ) << " r/min\n";
	
	mesh.load_text(ss.str().c_str(), 0.04);
	rmesh.add(mesh, glm::translate(mat, glm::vec3(0.5, 0, 0)));

	ss = std::stringstream();

	ss << "Local\n\n";
	ss << show( sys.loop.generator.get_rpm() / 60 ) << " Hz\n";
	Util::Streams::show_units( ss, sys.loop.generator.get_energy_generated() ) << "W\n";

	mesh.load_text(ss.str().c_str(), 0.04);
	rmesh.add(mesh, glm::translate(mat, glm::vec3(0.4, 0.7, 0)));

	ss = std::stringstream();

	ss << "Grid\n\n";
	ss << show( sys.grid.frequency ) << " Hz\n";

	mesh.load_text(ss.str().c_str(), 0.04);
	rmesh.add(mesh, glm::translate(mat, glm::vec3(0.7, 0.7, 0)));
}

void Turbine::remesh_fast(Mesh& rmesh)
{
	System& sys = *System::active;
	float off1 = sys.loop.generator.breaker_closed ? 0.07 : 0;
	rmesh.add(g_switch_breaker, glm::translate(glm::mat4(1), glm::vec3(0, off1, 0)));
}

void Turbine::render()
{
	double rpm = System::active->loop.generator.get_rpm();

	if(rpm > 3570 && rpm < 3630)
	{
		gm_synchroscope_dial.bind();
		gm_synchroscope_dial.uniform();
		gm_synchroscope_dial.render();
	}
}

