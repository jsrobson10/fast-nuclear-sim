
#pragma once

#include "../../graphics/data/mesh.hpp"

namespace Sim::Audio::Actions
{

struct Click
{
	unsigned int clip_ids[2];

	Click();

	void play(glm::mat4 mat);
	void play(const Graphics::Data::Mesh& mesh);
};

};

