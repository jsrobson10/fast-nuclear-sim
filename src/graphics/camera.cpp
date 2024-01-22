
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "camera.hpp"
#include "keyboard.hpp"

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace sim::graphics;

static double yaw = 0, pitch = 0;
static double x = 0, y = 0, z = 0;

void camera::rotate(double y, double p)
{
	yaw += y * 0.05;
	pitch += p * 0.05;

	if(pitch < -90) pitch = -90;
	if(pitch > 90) pitch = 90;
}

void camera::move(double xoff, double yoff, double zoff)
{
	x += xoff;
	y += yoff;
	z += zoff;
}

void camera::update()
{
	double xoff = 0, yoff = 0, zoff = 0;

	glm::vec<3, double> off(0, 0, 0);
	
	if(keyboard::is_pressed(GLFW_KEY_SPACE))
		off.y -= 1;
	if(keyboard::is_pressed(GLFW_KEY_LEFT_SHIFT))
		off.y += 1;
	if(keyboard::is_pressed(GLFW_KEY_W))
		off.z += 1;
	if(keyboard::is_pressed(GLFW_KEY_S))
		off.z -= 1;
	if(keyboard::is_pressed(GLFW_KEY_A))
		off.x += 1;
	if(keyboard::is_pressed(GLFW_KEY_D))
		off.x -= 1;

	double angle = -glm::radians<double>(yaw);

	glm::mat<2, 2, double> mat = {
		std::cos(angle), std::sin(angle),
		-std::sin(angle), std::cos(angle)
	};
	
	glm::vec<2, double> rotated = glm::vec<2, double>(off.x, off.z) * mat;
	
	y += off.y * 0.05;
	x += rotated.x * 0.05;
	z += rotated.y * 0.05;
}

glm::mat4 camera::get_model_matrix()
{
	glm::mat4 mat(1);

	mat = glm::rotate(mat, (float)glm::radians(pitch), glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, (float)glm::radians(yaw), glm::vec3(0, 1, 0));
	mat = glm::translate(mat, glm::vec3(x, y, z));

	return mat;
}

