
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "camera.hpp"
#include "input/keyboard.hpp"
#include "../math.hpp"

#include <cmath>
#include <iostream>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace sim::graphics;

static double yaw = 0, pitch = 0;
static glm::vec<3, double> pos(0, 0, 2);
static glm::vec<3, double> velocity(0);
static glm::mat4 camera_mat;

void camera::rotate(double y, double p)
{
	yaw += y * 0.05;
	pitch -= p * 0.05;

	if(pitch < 0) pitch = 0;
	if(pitch > 180) pitch = 180;
}

void camera::move(double xoff, double yoff, double zoff)
{
	pos.x += xoff;
	pos.y += yoff;
	pos.z += zoff;
}

void camera::update(double dt)
{
	glm::vec<2, double> off(0, 0);
	double m = 30;
	
	if(keyboard::is_pressed(GLFW_KEY_W))
		off.y += 1;
	if(keyboard::is_pressed(GLFW_KEY_S))
		off.y -= 1;
	if(keyboard::is_pressed(GLFW_KEY_A))
		off.x -= 1;
	if(keyboard::is_pressed(GLFW_KEY_D))
		off.x += 1;
	if(keyboard::is_pressed(GLFW_KEY_LEFT_SHIFT))
		m *= 1.5;
	if(off.x != 0 || off.y != 0)
		off = glm::normalize(off);

	double angle = glm::radians<double>(yaw);

	glm::mat<2, 2, double> mat = {
		std::cos(angle), std::sin(angle),
		-std::sin(angle), std::cos(angle)
	};
	
	glm::vec<2, double> rotated = glm::vec<2, double>(off.x, off.y) * mat;
	bool on_ground = false;
	
	velocity.z -= 9.81 * dt;

	if(pos.z + velocity.z * dt < 1.6)
	{
		on_ground = true;

		if(keyboard::is_pressed(GLFW_KEY_SPACE))
		{
			velocity.z = 3.5;
		}

		else
		{
			velocity.z = 0;
			pos.z = 1.6;
		}
	}

	else
	{
		m = 0;
	}
	
	velocity.x += rotated.x * m * dt;
	velocity.y += rotated.y * m * dt;

	double nx = pos.x + velocity.x * dt;
	double ny = pos.y + velocity.y * dt;

	if(nx > 8.9 || nx < -2.9)
		velocity.x = 0;
	if(std::abs(ny) > 3.9)
		velocity.y = 0;

	float m2 = std::pow(0.5, dt / (on_ground ? 0.05 : 1));

	pos += velocity * dt;
	velocity *= m2;

	camera_mat = glm::mat4(1);
	camera_mat = glm::rotate(camera_mat, (float)glm::radians(-pitch), glm::vec3(1, 0, 0));
	camera_mat = glm::rotate(camera_mat, (float)glm::radians(yaw), glm::vec3(0, 0, 1));
	camera_mat = glm::translate(camera_mat, glm::vec3(-pos.x, -pos.y, -pos.z));
}

glm::mat4 camera::get_matrix()
{
	return camera_mat;
}

