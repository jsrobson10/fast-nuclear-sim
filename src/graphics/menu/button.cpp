
#include "button.hpp"
#include "../data/font.hpp"
#include "../input/mouse.hpp"
#include "../input/focus.hpp"
#include "../window.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace Sim::Graphics::Menu;

const float FONT_SIZE = 0.06;

Button::Button(const std::string& text, glm::vec2 pos, std::function<void(Button&)> action)
	: pos(pos)
	, text(text)
	, action(action)
{
}

Button::Button(std::function<std::string(Button&)> get_text, glm::vec2 pos, std::function<void(Button&)> action)
	: get_text(get_text)
	, pos(pos)
	, action(action)
{
}

bool Button::check_hover()
{
	glm::vec2 mouse_pos = Mouse::get_normalized();

	float p0 = pos.x;
	float p1 = pos.y;
	float o0 = FONT_SIZE * 6;
	float o1 = FONT_SIZE;

	return (mouse_pos.x > p0-o0 && mouse_pos.x < p0+o0 && mouse_pos.y > p1-o1 && mouse_pos.y < p1+o1);
}

void Button::remesh_ui(Data::Mesh& rmesh)
{
	glm::vec4 colour;
	glm::vec2 mouse_pos = Mouse::get_normalized();

	float p0 = pos.x;
	float p1 = pos.y;
	float o0 = FONT_SIZE * 6;
	float o1 = FONT_SIZE;

	if(check_hover())
	{
		colour = {0.23, 0.23, 0.26, 1};
	}

	else
	{
		colour = {0.16, 0.16, 0.17, 1};
	}

	Data::Mesh::Primitive<4, 6> prim {
		.vertex_base = {
			.colour = colour,
			.material = {0, 0, 1},
		},
		.indices = {0, 1, 3, 0, 3, 2},
		.vertices = {
			{.pos = {p0-o0, p1-o1, 0}},
			{.pos = {p0-o0, p1+o1, 0}},
			{.pos = {p0+o0, p1-o1, 0}},
			{.pos = {p0+o0, p1+o1, 0}},
		},
	};
	
	rmesh.add(prim);

	Data::Fonts::BASE.load_text(rmesh, get_text ? get_text(*this) : text, {
		.size=FONT_SIZE,
		.align={0.5, 0.5},
		.mat=glm::translate(glm::mat4(1), glm::vec3(p0, p1, 0.01)),
	});
	
};

void Button::update()
{
	if(Focus::is_triggered(Focus::Trigger::MENU) && check_hover())
	{
		action(*this);
	}
}

