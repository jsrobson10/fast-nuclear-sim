
#pragma once

#include "../../graphics/data/mesh.hpp"

namespace Sim::Audio::Actions
{

struct Button
{
	unsigned int clip_ids[3];

	Button();

	void play(glm::mat4 mat);
	void play(const Graphics::Data::Mesh& mesh);
};

};

