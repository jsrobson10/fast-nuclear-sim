
#include "menu.hpp"
#include "menu_type.hpp"
#include "button.hpp"
#include "../data/font.hpp"
#include "../input/focus.hpp"
#include "../../system.hpp"

#include <vector>
#include <string>
#include <format>
#include <memory>
#include <fstream>

#include <json/json.h>
#include <glm/ext/matrix_transform.hpp>

using namespace Sim::Graphics::Menu;
using namespace Sim::Graphics;
using namespace Sim;

struct SlotsMenuWarning : public MenuType
{
	std::string path;

	std::vector<Button> buttons = {
		{"Back", {-0.5, 0.75}, [](Button& b) {
			Focus::clear_focus();
		}},
		{"Continue", {0.5, 0.75}, [this](Button& b) {
			Menu::open_new_game(path);
		}},
	};

	SlotsMenuWarning(std::string path) : path(path)
	{
		will_restore_later = false;
	}

	void remesh_ui(Data::Mesh& rmesh) override
	{
		Menu::remesh_ui(rmesh);

		Data::Fonts::BASE.load_text(rmesh, "Warning!\n\nCreating a new save on this slot will overwrite its contents.\nAre you sure you want to continue?", {
			.size = 0.08,
			.align = {0.5, 0.5},
			.mat = glm::translate(glm::mat4(1), {0, -0.2, 0}),
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

struct SlotsMenu : public MenuType
{
	std::vector<Button> buttons;
	bool is_new;

	SlotsMenu(bool is_new) : is_new(is_new)
	{
		for(int i = 0; i < 10; i++)
		{
			std::fstream file(std::format("saves/slot{}.json", i + 1));
			std::string status;
			bool exists;

			if(file.is_open())
			{
				Json::Value root;
				file >> root;

				double clock = root["clock"].asDouble();
				double days = std::floor(clock / (3600 * 24)) + 1;
				status = std::format("Day {}", days);
				exists = true;

				file.close();
			}

			else
			{
				status = "Empty";
				exists = false;
			}

			buttons.push_back({
				std::format("Slot {}: {}", i + 1, status),
				{i < 5 ? -0.5 : 0.5, -0.4 + 0.2 * (i % 5)},
				[this, exists, i](Button& b) {
					open_slot(i, exists);
				},
			});
		}

		buttons.push_back({
			"Back",
			{0, 0.75},
			[](Button& b) {
				Focus::clear_focus();
			},
		});
	}

	void open_slot(int i, bool exists)
	{
		std::string path = std::format("saves/slot{}.json", i + 1);

		if(is_new)
		{
			if(exists)
			{
				Focus::set(std::make_unique<SlotsMenuWarning>(path));
			}

			else
			{
				Menu::open_new_game(path);
			}
		}

		else if(exists)
		{
			Focus::clear_all();
			System::load(path);
		}
	}

	void remesh_ui(Data::Mesh& rmesh) override
	{
		Menu::remesh_ui(rmesh);

		std::string text = is_new ? "New Game" : "Load Saved";

		Data::Fonts::BASE.load_text(rmesh, text, {
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

void Menu::open_slots(bool is_new)
{
	Focus::set(std::make_unique<SlotsMenu>(is_new));
}

