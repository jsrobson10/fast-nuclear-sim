
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <json/json.h>

#include <fstream>
#include <memory>

#include "settings.hpp"
#include "window.hpp"
#include "resize.hpp"

using namespace Sim::Graphics;

static int msaa = 1;
static int text_refreshes = 1;
static int shadow_size = 1024;
static bool vsync = true;
static bool show_fps = false;
static bool show_debug = false;
static int cctv_size = 512;

template <typename T>
static T check(T value, T default_value)
{
	return value ? value : default_value;
}

void Settings::load()
{
	std::ifstream savefile("settings.json");

	if(!savefile)
	{
		save();
		return;
	}

	Json::Value root;
	savefile >> root;
	savefile.close();

	vsync = root["vsync"].asBool();
	msaa = check(root["msaa"].asInt(), 1);
	shadow_size = check(root["shadow_size"].asInt(), 1024);
	text_refreshes = check(root["text_refreshes"].asInt(), 4);
	show_fps = root["show_fps"].asBool();
	show_debug = root["show_debug"].asBool();
	cctv_size = check(root["cctv_size"].asInt(), 512);
	Resize::set_fullscreen(root["fullscreen"].asBool());
}

void Settings::save()
{
	Json::Value root;

	root["msaa"] = msaa;
	root["vsync"] = vsync;
	root["fullscreen"] = Resize::get_fullscreen();
	root["shadow_size"] = shadow_size;
	root["text_refreshes"] = text_refreshes;
	root["show_debug"] = show_debug;
	root["show_fps"] = show_fps;
	root["cctv_size"] = cctv_size;
	
	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "";

	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	std::ofstream savefile("settings.json");
	writer->write(root, &savefile);
	savefile.close();
}

int Settings::get_msaa() { return msaa; }
bool Settings::get_vsync() { return vsync; }
int Settings::get_shadow_size() { return shadow_size; }
bool Settings::get_show_fps() { return show_fps; }
int Settings::get_text_refreshes() { return text_refreshes; }
bool Settings::get_show_debug() { return show_debug; }
int Settings::get_cctv_size() { return cctv_size; }

void Settings::set_msaa(int p_msaa)
{
	msaa = p_msaa;
	Window::reload_rbos();
}

void Settings::set_vsync(bool p_vsync)
{
	vsync = p_vsync;
	glfwSwapInterval(vsync ? 1 : 0);
}

void Settings::set_shadow_size(int p_shadow_size)
{
	shadow_size = p_shadow_size;
}

void Settings::set_text_refreshes(int p_text_refreshes)
{
	text_refreshes = p_text_refreshes;
}

void Settings::set_show_fps(bool p_show_fps)
{
	show_fps = p_show_fps;
}

void Settings::set_show_debug(bool p_show_debug)
{
	show_debug = p_show_debug;
}

void Settings::set_cctv_size(int p_cctv_size)
{
	cctv_size = p_cctv_size;
}

