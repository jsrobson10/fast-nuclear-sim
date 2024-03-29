
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
	int cctv_size = Settings::get_cctv_size();
	int shadow_size = Settings::get_shadow_size();

	static constexpr double x_off = 0.5;

	std::vector<Button> buttons = {
		{"Back", {0, 0.75}, [](Button& b) {
			Focus::clear_focus();
		}},
		{[](Button& b) {
			return std::format("Fullscreen: {}", Resize::get_fullscreen() ? "On" : "Off");
		}, {-x_off, -0.3}, [](Button& b) {
			Resize::toggle_fullscreen();
		}},
		{[](Button& b) {
			return std::format("Vsync: {}", Settings::get_vsync() ? "On" : "Off");
		}, {-x_off, -0.1}, [](Button& b) {
			Settings::set_vsync(!Settings::get_vsync());
		}},
		{[](Button& b) {
			return std::format("Show FPS: {}", Settings::get_show_fps() ? "On" : "Off");
		}, {-x_off, 0.1}, [](Button& b) {
			Settings::set_show_fps(!Settings::get_show_fps());
		}},
		{[](Button& b) {
			return std::format("Show Debug: {}", Settings::get_show_debug() ? "On" : "Off");
		}, {-x_off, 0.3}, [](Button& b) {
			Settings::set_show_debug(!Settings::get_show_debug());
		}},
		{[](Button& b) {
			int msaa = Settings::get_msaa();
			return msaa > 1 ? std::format("MSAA: {}", msaa) : "MSAA: Off";
		}, {x_off, -0.3}, [this](Button& b) {

			int msaa = Settings::get_msaa() * 2;

			if(msaa > msaa_max)
			{
				msaa = 1;
			}

			Settings::set_msaa(msaa);
		}},
		{[this](Button& b) {
			return std::format("Shadows: {0}x{0}", shadow_size);
		}, {x_off, -0.1}, [this](Button& b) {
			shadow_size *= 2;

			if(shadow_size > std::min(texture_max, 4096))
			{
				shadow_size = 256;
			}
		}},
		{[this](Button& b) {
			return std::format("CCTV Size: {0}x{1}", cctv_size, cctv_size * 9 / 16);
		}, {x_off, 0.1}, [this](Button& b) {
			cctv_size *= 2;

			if(cctv_size > std::min(texture_max, 8192))
			{
				cctv_size = 256;
			}
		}},
		{[](Button& b) {
			return std::format("Text Refresh Rate: {}:1", Settings::get_text_refreshes());
		}, {x_off, 0.3}, [](Button& b) {
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
		Settings::set_shadow_size(shadow_size);
		Settings::set_cctv_size(cctv_size);
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

