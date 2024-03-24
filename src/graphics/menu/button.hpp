
#pragma once

#include "../data/mesh.hpp"

#include <glm/matrix.hpp>
#include <functional>
#include <string>

namespace Sim::Graphics::Menu
{

struct Button
{
	glm::vec2 pos;
	std::string text;
	std::function<std::string(Button&)> get_text;
	std::function<void(Button&)> action;

	Button(const std::string& text, glm::vec2 pos, std::function<void(Button&)> action);
	Button(std::function<std::string(Button&)> get_text, glm::vec2, std::function<void(Button&)> action);

	bool check_hover();
	void remesh_ui(Data::Mesh& rmesh);
	void update();
};

};

