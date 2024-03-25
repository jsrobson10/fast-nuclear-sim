
#pragma once

namespace Sim::Graphics::Settings
{

void load();
void save();

int get_msaa();
int get_shadow_size();
bool get_vsync();
bool get_show_fps();
int get_text_refreshes();
bool get_show_debug();
int get_cctv_size();

void set_msaa(int msaa);
void set_shadow_size(int shadow_size);
void set_vsync(bool vsync);
void set_show_fps(bool show_fps);
void set_text_refreshes(int text_refreshes);
void set_show_debug(bool show_debug);
void set_cctv_size(int cctv_size);

};

