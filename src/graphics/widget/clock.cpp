
#include "clock.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_transform.hpp>

#include <iomanip>
#include <sstream>
#include <cmath>

#include "../mesh/arrays.hpp"
#include "../mesh/font.hpp"
#include "../mesh/arrays.hpp"
#include "../resize.hpp"
#include "../../system.hpp"

using namespace sim::graphics::widget;

void Clock::update(double dt)
{
	Mesh m;
	double at = System::active.clock;
	glm::vec2 wsize(resize::get_size() / 2);
	std::stringstream ss;

	int t_s = std::fmod(at, 60);
	int t_m = std::fmod(at / 60, 60);
	int t_h = std::fmod(at / 3600, 24);

	ss << "Time: " << std::setfill('0') << std::setw(2) << t_h << ":";
   	ss << std::setfill('0') << std::setw(2) << t_m << ":";
	ss << std::setfill('0') << std::setw(2) << t_s << "\n";
	ss << "Day: " << std::floor(at / (3600 * 24)) << "\n";

	m.load_text(ss.str().c_str(), 20);
	
	data.bind();
	data.model_matrix = glm::translate(glm::mat4(1), glm::vec3(-wsize + glm::vec2(2, 2), 0));
	data.colour_matrix = arrays::colour({1, 1, 1, 1});
	data.set(m, GL_DYNAMIC_DRAW);
}

void Clock::render()
{
	data.bind();
	data.uniform();
	data.render();
}

