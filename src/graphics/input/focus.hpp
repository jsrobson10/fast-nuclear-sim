
#pragma once

namespace Sim::Graphics
{

namespace Focus
{
	enum Trigger
	{
		NONE = 0,
		INTERFACE = 1,
		MENU = 2,
		MENU_INPUT = 3,
	};
};

namespace Data
{
	struct Mesh;
};

};

#include "../data/mesh.hpp"

#include <glm/matrix.hpp>
#include <memory>

namespace Sim::Graphics::Focus
{

struct FocusType
{
	bool can_unfocus = true;
	bool cursor_is_visible = true;
	bool will_restore_later = true;
	bool will_advance_time = true;
	Trigger min_trigger_level = Trigger::INTERFACE;

	virtual ~FocusType() { }
	virtual void on_keypress(int key, int sc, int action, int mods) { }
	virtual void on_mouse_button(int button, int action, int mods) { }
	virtual void on_cursor_pos(double x, double y) { }
	virtual void on_charcode(unsigned int c) { }
	virtual void remesh_ui(Data::Mesh& rmesh) { }
	virtual void remesh(Data::Mesh& rmesh) { }
	virtual void update(double dt) { }
	virtual void render_ui() { }
	virtual void render() { }
};

bool is_focused();
void clear_focus();
void clear_all();
bool is_triggered(Trigger level = Trigger::NONE);
bool is_triggered_release();
bool should_advance_time();
glm::vec<3, double> get_trigger_near();
glm::vec<3, double> get_trigger_far();
void set(std::unique_ptr<FocusType> f);
void on_keypress(int key, int sc, int action, int mods);
void on_mouse_button(int button, int action, int mods);
void on_cursor_pos(double x, double y);
void on_charcode(unsigned int c);
void update(double dt);
void remesh_ui(Data::Mesh& rmesh);
void remesh(Data::Mesh& rmesh);
void render_ui();
void render();

};

