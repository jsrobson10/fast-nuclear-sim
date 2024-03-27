
#include <AL/alut.h>

#include "click.hpp"
#include "../clip.hpp"
#include "../source.hpp"
#include "../../util/random.hpp"

using namespace Sim::Audio::Actions;

Click::Click()
{
	clip_ids[0] = Clip::load("click_1.ogg");
	clip_ids[1] = Clip::load("click_2.ogg");
}

void Click::play(glm::mat4 mat)
{
	Source source(clip_ids[Util::Random::randint(0, 1)]);
	source.mat = mat;
	source.play();
}

void Click::play(const Graphics::Data::Mesh& mesh)
{
	play(mesh.transforms[0]);
}

