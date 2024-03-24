
#include "clock.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_transform.hpp>

#include <iomanip>
#include <sstream>
#include <cmath>

#include "../data/arrays.hpp"
#include "../data/font.hpp"
#include "../data/arrays.hpp"
#include "../resize.hpp"
#include "../../system.hpp"
#include "../../util/streams.hpp"

using namespace Sim::Graphics::Widget;
using namespace Sim::Graphics::Data;

void Clock::update(double dt)
{
	this->dt = dt;
}

void Clock::remesh_ui(Mesh& rmesh)
{
	glm::vec2 wsize(Resize::get_size() / 2);

	if(i++ % 4 == 0)
	{
		double at = System::active->clock;
		std::stringstream ss;

		int t_s = std::fmod(at, 60);
		int t_m = std::fmod(at / 60, 60);
		int t_h = std::fmod(at / 3600, 24);

		ss << "FPS: " << (1.0 / dt) << "\n";
		ss << "Time: " << std::setfill('0') << std::setw(2) << t_h << ":";
   		ss << std::setfill('0') << std::setw(2) << t_m << ":";
		ss << std::setfill('0') << std::setw(2) << t_s << "\n";
		ss << "Day: " << std::floor(at / (3600 * 24)) << "\n";

		text = ss.str();
	}
	
	glm::mat4 mat(1);
	mat = glm::scale(mat, glm::vec3(1 / wsize.y, 1 / wsize.y, 1));
	mat = glm::translate(mat, glm::vec3(-wsize + glm::vec2(2, 2), -0.99));

	Fonts::BASE.load_text(rmesh, text, {.size=20, .mat=mat, .bake=true});
}

