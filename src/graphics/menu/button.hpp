
#pragma once

#include "../data/mesh.hpp"

#include <glm/matrix.hpp>
#include <functional>

namespace Sim::Graphics::Menu
{

struct Button
{
	const char* text;
	glm::vec2 pos;
	std::function<void()> action;

	Button(const char* text, glm::vec2 pos, std::function<void()> action);

	bool check_hover();
	void remesh_ui(Data::Mesh& rmesh);
	void update();
};

};

