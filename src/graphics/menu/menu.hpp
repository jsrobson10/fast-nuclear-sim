
#pragma once

#include "../data/mesh.hpp"

#include <string>

namespace Sim::Graphics::Menu
{

void remesh_ui(Data::Mesh& rmesh);

void open_pause();
void open_settings();
void open_main();
void open_slots(bool load);
void open_new_game(const std::string& path);

};

