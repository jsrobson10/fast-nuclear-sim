
#include <AL/alut.h>

#include <vorbis/vorbisfile.h>

#include "clip.hpp"

#include <vector>
#include <stdexcept>
#include <string>
#include <format>
#include <unordered_map>

using namespace Sim::Audio;

static std::unordered_map<std::string, unsigned int> all_clips;

unsigned int Clip::load(const std::string& name)
{
	auto it = all_clips.find(name);

	if(it != all_clips.end())
	{
		return it->second;
	}

	OggVorbis_File oggFile;
	std::string path = std::format("../assets/audio/{}", name);

	if(ov_fopen(path.c_str(), &oggFile) < 0)
	{
		throw std::runtime_error("Failed to open Ogg Vorbis file");
	}

	vorbis_info* info = ov_info(&oggFile, -1);

	ALenum format = info->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	ALsizei freq = info->rate;

	std::vector<char> buffer;
	char tempBuffer[4096];
	int bitstream;

	for(;;)
	{
		long bytesRead = ov_read(&oggFile, tempBuffer, sizeof(tempBuffer), 0, 2, 1, &bitstream);

		if(bytesRead > 0)
		{
			buffer.insert(buffer.end(), tempBuffer, tempBuffer + bytesRead);
		}

		else if(bytesRead < 0)
		{
			ov_clear(&oggFile);
			throw std::runtime_error("Failed to read Ogg Vorbis file");
		}

		else
		{
			break;
		}
	}

	ov_clear(&oggFile);

	unsigned int buffid;
	alGenBuffers(1, &buffid);
	alBufferData(buffid, format, buffer.data(), buffer.size(), freq);
	all_clips.emplace(name, buffid);
	return buffid;
}

