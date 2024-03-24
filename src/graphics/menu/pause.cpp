
#include "menu.hpp"
#include "pause.hpp"
#include "../input/focus.hpp"
#include "../data/font.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <memory>

using namespace Sim::Graphics::Menu;
using namespace Sim::Graphics;

struct PauseMenu : public MenuType
{
	void remesh_ui(Data::Mesh& rmesh) override
	{
		Menu::remesh_ui(rmesh);

		Data::Fonts::BASE.load_text(rmesh, "Game Paused", {
			.size = 0.08,
			.align = {0.5, 0.5},
			.mat = glm::translate(glm::mat4(1), {0, -0.5, 0}),
		});
	}
};

void Pause::open()
{
	Focus::set(std::make_unique<PauseMenu>());
}

