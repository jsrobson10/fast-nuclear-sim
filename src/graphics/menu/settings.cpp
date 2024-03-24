
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "button.hpp"
#include "menu.hpp"
#include "menu_type.hpp"
#include "../input/focus.hpp"
#include "../data/font.hpp"
#include "../window.hpp"
#include "../settings.hpp"
#include "../resize.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <memory>
#include <vector>
#include <string>
#include <format>

using namespace Sim::Graphics;
using namespace Sim::Graphics::Menu;

struct SettingsMenu : public MenuType
{
	int msaa_max;
	int texture_max;
	bool fullscreen = Resize::get_fullscreen();

	std::vector<Button> buttons = {
		{"Back", {0, 0.75}, [](Button& b) {
			Focus::clear_focus();
		}},
		{[](Button& b) {
			return std::format("Fullscreen: {}", Resize::get_fullscreen() ? "On" : "Off");
		}, {-0.75, -0.5}, [](Button& b) {
			Resize::toggle_fullscreen();
		}},
		{[](Button& b) {
			return std::format("Vsync: {}", Settings::get_vsync() ? "On" : "Off");
		}, {-0.75, -0.3}, [](Button& b) {
			Settings::set_vsync(!Settings::get_vsync());
		}},
		{[](Button& b) {
			return std::format("Show FPS: {}", Settings::get_show_fps() ? "On" : "Off");
		}, {-0.75, -0.1}, [](Button& b) {
			Settings::set_show_fps(!Settings::get_show_fps());
		}},
		{[](Button& b) {
			int msaa = Settings::get_msaa();
			return msaa > 1 ? std::format("MSAA: {}", msaa) : "MSAA: Off";
		}, {-0.75, 0.1}, [this](Button& b) {

			int msaa = Settings::get_msaa() * 2;

			if(msaa > msaa_max)
			{
				msaa = 1;
			}

			Settings::set_msaa(msaa);
		}},
		{[](Button& b) {
			return std::format("Shadows: {0}x{0}", Settings::get_shadow_size());
		}, {-0.75, 0.3}, [this](Button& b) {
			int size = Settings::get_shadow_size() * 2;

			if(size > std::min(texture_max, 4096))
			{
				size = 256;
			}

			Settings::set_shadow_size(size);
		}},
		{[](Button& b) {
			return std::format("Text Refresh Rate: {}:1", Settings::get_text_refreshes());
		}, {-0.75, 0.5}, [](Button& b) {
			int rate = Settings::get_text_refreshes() / 2;

			if(rate < 1)
			{
				rate = 16;
			}

			Settings::set_text_refreshes(rate);
		}},
	};

	SettingsMenu()
	{
		glGetIntegerv(GL_MAX_SAMPLES, &msaa_max);
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texture_max);
	}

	~SettingsMenu()
	{
		Settings::save();
	}

	void remesh_ui(Data::Mesh& rmesh) override
	{
		Menu::remesh_ui(rmesh);

		Data::Fonts::BASE.load_text(rmesh, "Settings", {
			.size = 0.08,
			.align = {0.5, 0.5},
			.mat = glm::translate(glm::mat4(1), {0, -0.75, 0}),
		});

		for(Button& b : buttons)
		{
			b.remesh_ui(rmesh);
		}
	}

	void update(double dt) override
	{
		bool fullscreen = Resize::get_fullscreen();

		if(fullscreen != this->fullscreen)
		{
			this->fullscreen = fullscreen;
			buttons[1].text = buttons[1].get_text(buttons[1]);
		}

		for(Button& b : buttons)
		{
			b.update();
		}
	}
};

void Menu::open_settings()
{
	Focus::set(std::make_unique<SettingsMenu>());
}

