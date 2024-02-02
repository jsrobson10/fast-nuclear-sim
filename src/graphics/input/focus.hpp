
#pragma once

#include <glm/matrix.hpp>

#include <memory>

namespace sim::graphics::focus
{

struct focus_t
{
	virtual ~focus_t() { }
	virtual bool cursor_is_visible() { return true; }
	virtual void on_keypress(int key, int sc, int action, int mods) { }
	virtual void on_mouse_button(int button, int action, int mods) { }
	virtual void on_cursor_pos(double x, double y) { }
	virtual void on_charcode(unsigned int c) { }
	virtual void update() { }
};

bool is_focused();
void clear_focus();
bool is_triggered();
bool is_mouse_locked();
void clear_mouse_locked();
glm::vec<3, double> get_trigger_near();
glm::vec<3, double> get_trigger_far();
void set(std::unique_ptr<focus_t> f);
void on_keypress(int key, int sc, int action, int mods);
void on_mouse_button(int button, int action, int mods);
void on_cursor_pos(double x, double y);
void on_charcode(unsigned int c);
void update();

};

