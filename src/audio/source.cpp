
#include <AL/alut.h>

#include <string>
#include <unordered_map>

#include "source.hpp"
#include "sound_engine.hpp"
#include "clip.hpp"

using namespace Sim::Audio;

Source::Source(const std::string& clip_name) : Source(Clip::load(clip_name))
{
}

Source::Source(unsigned int clip_id)
{
	SoundEngine::source_register(this);
	alSourcei(id, AL_BUFFER, clip_id);
}

Source::Source(Source&& o) : id(o.id), se_id(o.se_id)
{
	SoundEngine::source_reregister(&o, this);
	o.se_id = -1;
}

Source::~Source()
{
	if(se_id != -1) SoundEngine::source_unregister(this);
}

void Source::process()
{
	SoundEngine::source_process(this);
}

void Source::loop()
{
	alSourcei(id, AL_LOOPING, AL_TRUE);
	alSourcePlay(id);
}

void Source::play()
{
	alSourcePlay(id);
}

void Source::stop()
{
	alSourceStop(id);
}

void Source::pause()
{
	alSourcePause(id);
}

void Source::rewind()
{
	alSourceRewind(id);
}

