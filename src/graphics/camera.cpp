
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "camera.hpp"
#include "keyboard.hpp"

#include <iostream>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace sim::graphics;

static double yaw = 0, pitch = 0;
static glm::vec<3, double> pos(56, 90, 8);
static glm::vec<3, double> velocity(0);

void camera::rotate(double y, double p)
{
	yaw -= y * 0.05;
	pitch += p * 0.05;

	if(pitch < 0) pitch = 0;
	if(pitch > 180) pitch = 180;
}

void camera::move(double xoff, double yoff, double zoff)
{
	pos.x += xoff;
	pos.y += yoff;
	pos.z += zoff;
}

void camera::update()
{
	glm::vec<2, double> off(0, 0);
	double m = 0.01;
	
	if(keyboard::is_pressed(GLFW_KEY_W))
		off.y += 1;
	if(keyboard::is_pressed(GLFW_KEY_S))
		off.y -= 1;
	if(keyboard::is_pressed(GLFW_KEY_A))
		off.x += 1;
	if(keyboard::is_pressed(GLFW_KEY_D))
		off.x -= 1;
	if(keyboard::is_pressed(GLFW_KEY_LEFT_SHIFT))
		m *= 1.5;
	if(off.x != 0 || off.y != 0)
		off /= std::sqrt(off.x * off.x + off.y * off.y);

	double angle = glm::radians<double>(yaw);

	glm::mat<2, 2, double> mat = {
		std::cos(angle), std::sin(angle),
		-std::sin(angle), std::cos(angle)
	};
	
	glm::vec<2, double> rotated = glm::vec<2, double>(off.x, off.y) * mat;
	bool on_ground = false;
	
	velocity.z -= 0.00981;

	if(pos.z + velocity.z < 3.5)
	{
		on_ground = true;

		if(keyboard::is_pressed(GLFW_KEY_SPACE))
		{
			velocity.z += 0.4;
		}

		else
		{
			velocity.z = 0;
		}
	}

	else
	{
		m = 0;
	}
	
	velocity.x += rotated.x * m;
	velocity.y += rotated.y * m;

	pos += velocity;
	velocity *= glm::vec<3, double>(on_ground ? 0.9 : 0.9999);
}

glm::mat4 camera::get_model_matrix()
{
	glm::mat4 mat(1);

	mat = glm::rotate(mat, (float)glm::radians(pitch), glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, (float)glm::radians(yaw), glm::vec3(0, 0, 1));
	mat = glm::translate(mat, glm::vec3(pos.x, pos.y, pos.z));

	return mat;
}

