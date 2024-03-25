
#include "button.hpp"
#include "menu.hpp"
#include "menu_type.hpp"
#include "../input/focus.hpp"
#include "../data/font.hpp"
#include "../window.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <memory>
#include <vector>

using namespace Sim;
using namespace Sim::Graphics;
using namespace Sim::Graphics::Menu;

struct PauseMenu : public MenuType
{
	std::vector<Button> buttons = {
		{"Resume", {0, 0}, [](Button& b) {
			Focus::clear_focus();
		}},
		{"Settings", {0, 0.2}, [](Button& b) {
			open_settings();
		}},
		{"Quit", {0, 0.4}, [](Button& b) {
			System::save();
			Focus::clear_all();
			open_main();
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

void Menu::open_pause()
{
	Focus::set(std::make_unique<PauseMenu>());
}

