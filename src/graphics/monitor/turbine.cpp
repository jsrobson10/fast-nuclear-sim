
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "turbine.hpp"
#include "../../system.hpp"
#include "../../coolant/valve.hpp"
#include "../input/focus.hpp"
#include "../../util/streams.hpp"
#include "../../util/math.hpp"
#include "../data/font.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <iostream>

using namespace Sim::Graphics;
using namespace Sim::Graphics::Monitor;
using namespace Sim::Graphics::Data;
using namespace Sim::Util::Streams;

Turbine::Turbine(const Model& model)
	: Data::MeshGen("turbine")
	, a_click(model, {"visual_breaker_switch"}, {"click_1.ogg", "click_2.ogg"})
{
	mat = model.load_matrix("translation_monitor_5");

	g_dial_phase = model.load("visual_dial_phase");
	g_dial_voltage = model.load("visual_dial_voltage");
	g_dial_power = model.load("visual_dial_power");
	g_dial_frequency = model.load("visual_dial_frequency");

	g_switch_breaker = model.load("visual_breaker_switch");
	m_switch_breaker = model.load("click_breaker_switch");
}

void Turbine::update(double dt)
{
	System& sys = *System::active;

	if(m_switch_breaker.check_focus(Focus::Trigger::INTERFACE)) {
		sys.loop.generator.breaker_closed = !sys.loop.generator.breaker_closed;
		a_click.play(0);
	}
}

void Turbine::get_static_transforms(std::vector<glm::mat4>& transforms)
{
	System& sys = *System::active;
	double freq = sys.loop.generator.get_frequency();
	glm::mat4 mat_phase(1);
	glm::mat4 mat_voltage(1);
	glm::mat4 mat_power(1);
	glm::mat4 mat_frequency(1);

	if(freq > 59 && freq < 61)
	{
		mat_phase = glm::rotate(mat_phase, float(sys.loop.generator.get_phase_diff()), glm::vec3(0, 1, 0));
	}

	mat_voltage = glm::rotate(mat_voltage, float(Util::Math::map(sys.loop.generator.get_voltage(), 0, 24e3, 0, M_PI)), glm::vec3(0, 1, 0));
	mat_power = glm::rotate(mat_power, float(Util::Math::map(sys.loop.generator.get_power(), 0, 600e6, 0, M_PI)), glm::vec3(0, 1, 0));
	mat_frequency = glm::rotate(mat_frequency, float(Util::Math::map(sys.loop.generator.get_frequency(), 0, 120, 0, M_PI)), glm::vec3(0, 1, 0));

	transforms.push_back(mat_phase);
	transforms.push_back(mat_voltage);
	transforms.push_back(mat_power);
	transforms.push_back(mat_frequency);
	
	float off1 = sys.loop.generator.breaker_closed ? 0.07 : 0;

	transforms.push_back(glm::translate(glm::mat4(1), glm::vec3(0, off1, 0)));
}

void Turbine::remesh_static(Mesh& rmesh)
{
	std::stringstream ss;

	ss << "Turbine\n\n";
	ss << "Heat\nPressure\nSpeed\n\n";

	Data::Fonts::BASE.load_text(rmesh, ss.str(), {.size=0.04, .mat=mat, .bake=true});
	
	rmesh.add(g_dial_phase);
	rmesh.add(g_dial_voltage);
	rmesh.add(g_dial_power);
	rmesh.add(g_dial_frequency);
	rmesh.add(g_switch_breaker);
}

void Turbine::remesh_slow(Mesh& rmesh)
{
	std::stringstream ss;
	System& sys = *System::active;

	ss << "\n\n";
	ss << show( sys.loop.turbine.get_heat() ) << " C\n";
	ss << show( sys.loop.turbine.get_pressure() / 1000 ) << " kPa\n";
	ss << show( sys.loop.generator.get_rpm() ) << " r/min\n";
	
	Data::Fonts::BASE.load_text(rmesh, ss.str(), {.size=0.04, .mat=glm::translate(mat, glm::vec3(0.5, 0, 0)), .bake=true});
}

