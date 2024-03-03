
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

Turbine::Turbine(const Model& model)
{
	mat = Locations::monitors[4];

	g_synchroscope_dial = model.load("visual_synchroscope_dial");
	g_switch_breaker = model.load("visual_breaker_switch");
	m_switch_breaker = model.load("click_breaker_switch");
	
	g_synchroscope_dial.set_transform_id();
	g_switch_breaker.set_transform_id();
}

void Turbine::update(double dt)
{
	System& sys = *System::active;

	if(m_switch_breaker.check_focus())
		sys.loop.generator.breaker_closed = !sys.loop.generator.breaker_closed;
}

void Turbine::get_static_transforms(std::vector<glm::mat4>& transforms)
{
	System& sys = *System::active;
	double rpm = sys.loop.generator.get_rpm();
	glm::mat4 mat(1);

	if(rpm > 3570 && rpm < 3630)
	{
		mat = glm::translate(mat, glm::vec3(6.35, 3.949, 1.35));
		mat = glm::rotate(mat, float(sys.loop.generator.get_phase_diff()), glm::vec3(0, 1, 0));
		mat = glm::translate(mat, glm::vec3(-6.35, -3.949, -1.35));
	}
	
	float off1 = sys.loop.generator.breaker_closed ? 0.07 : 0;

	transforms.push_back(mat);
	transforms.push_back(glm::translate(glm::mat4(1), glm::vec3(0, off1, 0)));
}

void Turbine::remesh_static(Mesh& rmesh)
{
	std::stringstream ss;
	Sim::Graphics::Mesh mesh;

	ss << "Turbine\n\n";
	ss << "Heat\nPressure\nSpeed\n\n";

	mesh.load_text(ss.str().c_str(), 0.04);
	rmesh.add(mesh, mat);
	
	mesh.load_text("Synchroscope", 0.04);
	rmesh.add(mesh, glm::translate(mat, glm::vec3(0, 0.6, 0)));

	rmesh.add(g_synchroscope_dial);
	rmesh.add(g_switch_breaker);
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
}

