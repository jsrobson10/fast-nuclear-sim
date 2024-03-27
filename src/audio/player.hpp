
#pragma once

#include <glm/matrix.hpp>
#include "../util/random.hpp"
#include "../graphics/data/model.hpp"
#include "source.hpp"
#include "clip.hpp"

#include <string>
#include <array>

namespace Sim::Audio
{

template <int MATRICES, int SOURCES>
struct Player
{
	glm::mat4 matrices[MATRICES];
	unsigned int sources[SOURCES];
	float pitch = 1;
	float gain = 1;

	Player(const Graphics::Data::Model& model, const std::array<const char*, MATRICES>& matrix_names, const std::array<std::string, SOURCES>& source_names)
	{
		for(int i = 0; i < MATRICES; i++)
		{
			matrices[i] = model.load_matrix(matrix_names[i]);
		}

		for(int i = 0; i < SOURCES; i++)
		{
			sources[i] = Clip::load(source_names[i]);
		}
	}

	Source get(int id)
	{
		Source s(sources[Util::Random::randint(0, SOURCES - 1)]);
		s.mat = matrices[id];
		s.pitch = pitch;
		s.gain = gain;
		return s;
	}

	void play(int id)
	{
		Source s = get(id);
		s.process();
		s.play();
	}
};

};

