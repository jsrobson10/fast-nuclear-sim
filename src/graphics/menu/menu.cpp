
#include "menu.hpp"
#include "menu_type.hpp"
#include "../resize.hpp"

#include <iostream>

using namespace Sim::Graphics;

void Menu::remesh_ui(Data::Mesh& rmesh)
{
	float p0 = Resize::get_aspect();
	float p1 = 1;
	float p2 = -0.98;

	rmesh.add(Data::Mesh::Primitive<4, 6>{
		.vertex_base = {
			.colour = {0, 0, 0, 0.5},
			.material = {0, 0, 1}
		},
		.indices = {0, 1, 3, 0, 3, 2},
		.vertices = {
			{.pos = {-p0, -p1, p2}},
			{.pos = {-p0,  p1, p2}},
			{.pos = { p0, -p1, p2}},
			{.pos = { p0,  p1, p2}},
		},
	});
}

Menu::MenuType::MenuType()
{
	cursor_is_visible = true;
	will_advance_time = false;
	min_trigger_level = Focus::Trigger::MENU;
}

