
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "camera.hpp"
#include "data/mesh.hpp"
#include "input/keyboard.hpp"
#include "../util/math.hpp"
#include "input/focus.hpp"

#include <cmath>
#include <iostream>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace Sim::Graphics;

static bool in_main_menu = true;
static bool on_ground = false;
static double yaw = 0, pitch = 90;
static glm::vec<3, double> pos(0, 0, 2);
static glm::vec<3, double> velocity(0);
static Data::Mesh collision_scene;
static Data::Camera camera_main_menu;
static glm::mat4 camera_mat;
static glm::mat4 proj_mat;

void Camera::reset()
{
	on_ground = false;
	yaw = 0;
	pitch = 90;
	pos = glm::vec<3, double>(0, 0, 2);
	velocity = glm::vec<3, double>(0);
}

bool Camera::is_in_main_menu()
{
	return in_main_menu;
}

void Camera::set_in_main_menu(bool b)
{
	in_main_menu = b;
}

void Camera::set_pos(glm::vec<3, double> p)
{
	pos = p;
}

glm::vec<3, double> Camera::get_velocity()
{
	return velocity;
}

Json::Value Camera::serialize()
{
	Json::Value node;

	node["on_ground"] = on_ground;
	node["yaw"] = yaw;
	node["pitch"] = pitch;
	node["pos"]["x"] = pos[0];
	node["pos"]["y"] = pos[1];
	node["pos"]["z"] = pos[2];
	node["velocity"]["x"] = velocity[0];
	node["velocity"]["y"] = velocity[1];
	node["velocity"]["z"] = velocity[2];

	return node;
}

void Camera::load(const Json::Value& node)
{
	on_ground = node["on_ground"].asBool();
	yaw = node["yaw"].asDouble();
	pitch = node["pitch"].asDouble();
	pos[0] = node["pos"]["x"].asDouble();
	pos[1] = node["pos"]["y"].asDouble();
	pos[2] = node["pos"]["z"].asDouble();
	velocity[0] = node["velocity"]["x"].asDouble();
	velocity[1] = node["velocity"]["y"].asDouble();
	velocity[2] = node["velocity"]["z"].asDouble();
}

void Camera::rotate(double y, double p)
{
	pitch -= p * 0.05;
	if(pitch < 0) pitch = 0;
	if(pitch > 180) pitch = 180;

	yaw = Util::Math::mod(yaw + y * 0.05, 360);
}

void Camera::move(double xoff, double yoff, double zoff)
{
	pos.x += xoff;
	pos.y += yoff;
	pos.z += zoff;
}

glm::vec<3, double> Camera::get_normal()
{
	glm::mat<3, 3, double> mat(camera_mat);
	return glm::vec<3, double>(0, 0, -1) * mat;
}

glm::vec<3, double> Camera::get_pos()
{
	return pos;
}

glm::vec<3, double> Camera::get_pos_base()
{
	return pos - glm::vec<3, double>(0, 0, 1.5);
}

void Camera::init(Data::Model& model)
{
	collision_scene = model.load("collision");
	camera_main_menu = model.extract_camera("MainMenu");
}

void Camera::update(double dt)
{
	if(Focus::should_advance_time())
	{
		glm::vec<2, double> off(0, 0);
		double m = 30;
		
		if(Keyboard::is_pressed(GLFW_KEY_W))
			off.y += 1;
		if(Keyboard::is_pressed(GLFW_KEY_S))
			off.y -= 1;
		if(Keyboard::is_pressed(GLFW_KEY_A))
			off.x -= 1;
		if(Keyboard::is_pressed(GLFW_KEY_D))
			off.x += 1;
		if(Keyboard::is_pressed(GLFW_KEY_LEFT_SHIFT))
			m *= 1.5;
		if(off.x != 0 || off.y != 0)
			off = glm::normalize(off);

		double angle = glm::radians<double>(yaw);

		glm::mat<2, 2, double> mat = {
			std::cos(angle), std::sin(angle),
			-std::sin(angle), std::cos(angle)
		};
		
		glm::vec<2, double> rotated = glm::vec<2, double>(off.x, off.y) * mat;
		velocity.z -= 9.81 * dt;

		if(on_ground)
		{
			velocity.x += rotated.x * m * dt;
			velocity.y += rotated.y * m * dt;
		}
		
		if(on_ground && Keyboard::is_pressed(GLFW_KEY_SPACE))
		{
			velocity.z += 3.5;
		}

		glm::vec<3, double> velocity2;
   
		velocity2 = collision_scene.calc_intersect(pos, velocity * dt);
		velocity2 = collision_scene.calc_intersect(pos + glm::vec<3, double>(0, 0, -1.5), velocity2) / dt;

		pos += velocity2 * dt;
		on_ground = ((velocity * dt / dt).z != velocity2.z);
		velocity = velocity2 * std::pow(0.5, dt / (on_ground ? 0.05 : 10));
	}

	if(in_main_menu)
	{
		glm::vec3 pos = camera_main_menu.pos;
		glm::vec3 look = camera_main_menu.look;
		glm::vec3 up = camera_main_menu.up;

		camera_mat = glm::lookAt(pos, pos + look, up);
	}

	else
	{
		camera_mat = glm::mat4(1);
		camera_mat = glm::rotate(camera_mat, (float)glm::radians(-pitch), glm::vec3(1, 0, 0));
		camera_mat = glm::rotate(camera_mat, (float)glm::radians(yaw), glm::vec3(0, 0, 1));
		camera_mat = glm::translate(camera_mat, glm::vec3(-pos.x, -pos.y, -pos.z));
	}
}

float Camera::get_fov()
{
	if(in_main_menu)
	{
		return camera_main_menu.fov;
	}

	else
	{
		return glm::radians(90.f);
	}
}

double Camera::get_pitch()
{
	return pitch;
}

double Camera::get_yaw()
{
	return yaw;
}

glm::mat4 Camera::get_matrix()
{
	return camera_mat;
}

