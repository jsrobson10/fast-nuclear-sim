
#pragma once

#include <memory>

namespace sim::graphics::focus
{

struct focus_t
{
	virtual ~focus_t() { }
	virtual void on_keypress(int key, int sc, int action, int mods) { }
	virtual void on_mouse_button(int button, int action, int mods) { }
	virtual void on_cursor_pos(double x, double y) { }
	virtual void on_charcode(unsigned int c) { }
	virtual void update() { }
};

void clear();
bool is_focused();
bool is_triggered();
void set(std::unique_ptr<focus_t> f);
void on_keypress(int key, int sc, int action, int mods);
void on_mouse_button(int button, int action, int mods);
void on_cursor_pos(double x, double y);
void on_charcode(unsigned int c);
void update();

};

