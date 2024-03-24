
#include "menu.hpp"
#include "pause.hpp"
#include "button.hpp"
#include "../input/focus.hpp"
#include "../data/font.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <memory>
#include <vector>

using namespace Sim::Graphics::Menu;
using namespace Sim::Graphics;

struct PauseMenu : public MenuType
{
	std::vector<Button> buttons = {
		{"Resume", {0, 0}, []() {
			Focus::clear_focus();
		}},
		{"Settings", {0, 0.2}, []() {
			// TODO
		}},
		{"Exit", {0, 0.4}, []() {
			// TODO
		}},
	};

	void remesh_ui(Data::Mesh& rmesh) override
	{
		Menu::remesh_ui(rmesh);

		Data::Fonts::BASE.load_text(rmesh, "Game Paused", {
			.size = 0.08,
			.align = {0.5, 0.5},
			.mat = glm::translate(glm::mat4(1), {0, -0.5, 0}),
		});

		for(Button& b : buttons)
		{
			b.remesh_ui(rmesh);
		}
	}

	void update(double dt) override
	{
		for(Button& b : buttons)
		{
			b.update();
		}
	}
};

void Pause::open()
{
	Focus::set(std::make_unique<PauseMenu>());
}

