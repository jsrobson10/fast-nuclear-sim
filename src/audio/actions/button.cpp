
#include <AL/alut.h>

#include "button.hpp"
#include "../clip.hpp"
#include "../source.hpp"
#include "../../util/random.hpp"

using namespace Sim::Audio::Actions;

Button::Button()
{
	clip_ids[0] = Clip::load("button_1.ogg");
	clip_ids[1] = Clip::load("button_2.ogg");
	clip_ids[1] = Clip::load("button_3.ogg");
}

void Button::play(glm::mat4 mat)
{
	Source source(clip_ids[Util::Random::randint(0, 2)]);
	source.mat = mat;
	source.play();
}

void Button::play(const Graphics::Data::Mesh& mesh)
{
	play(mesh.transforms[0]);
}

