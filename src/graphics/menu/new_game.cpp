
#include "menu.hpp"
#include "menu_type.hpp"
#include "button.hpp"
#include "../data/font.hpp"
#include "../input/focus.hpp"
#include "../../system.hpp"
#include "../camera.hpp"

#include <vector>

#include <glm/gtc/matrix_transform.hpp>

using namespace Sim::Graphics::Menu;
using namespace Sim::Graphics;
using namespace Sim;

struct NewGameMenu : public MenuType
{

};

void Menu::open_new_game(const std::string& path)
{
	System::active = std::make_unique<System>();
	System::active->path = path;
	Focus::clear_all();
	Camera::reset();
}

