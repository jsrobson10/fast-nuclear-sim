
#pragma once

#include "../data/mesh.hpp"
#include "../input/focus.hpp"

namespace Sim::Graphics::Menu
{
	void remesh_ui(Data::Mesh& rmesh);

	struct MenuType : public Focus::FocusType
	{
		MenuType();
	};
};

