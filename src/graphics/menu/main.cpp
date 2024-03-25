
#include "menu.hpp"
#include "menu_type.hpp"
#include "button.hpp"
#include "../camera.hpp"
#include "../window.hpp"
#include "../data/font.hpp"

#include <vector>

#include <glm/gtc/matrix_transform.hpp>

using namespace Sim::Graphics::Menu;
using namespace Sim::Graphics;

static Data::Camera camera;

struct MainMenu : public MenuType
{
	std::vector<Button> buttons {
		{"Load New", {0, 0}, [](Button& b) {
			open_slots(true);
		}},
		{"Load Saved", {0, 0.2}, [](Button& b) {
			open_slots(false);
		}},
		{"Settings", {0, 0.4}, [](Button& b) {
			open_settings();
		}},
		{"Exit", {0, 0.6}, [](Button& b) {
			Window::close();
		}},
	};

	MainMenu()
	{
		can_unfocus = false;
		Camera::set_in_main_menu(true);
	}

	~MainMenu()
	{
		Camera::set_in_main_menu(false);
	}

	void remesh_ui(Data::Mesh& rmesh) override
	{
		Menu::remesh_ui(rmesh);

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

void Menu::open_main()
{
	Focus::set(std::make_unique<MainMenu>());
}

