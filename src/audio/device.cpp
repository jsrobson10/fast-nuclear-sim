
#include <AL/alut.h>

#include <stdexcept>
#include <iostream>

#include "device.hpp"
#include "sound_engine.hpp"

using namespace Sim::Audio;

static ALCdevice* device = nullptr;
static ALCcontext* context = nullptr;

void Device::init(int argc, char** argv)
{
	alutInit(&argc, argv);

	if(!(device = alcOpenDevice(nullptr)))
	{
		throw std::runtime_error("Failed to open audio device");
	}

	if(!(context = alcCreateContext(device, nullptr)))
	{
		alcCloseDevice(device);
		throw std::runtime_error("Failed to create audio context");
	}

	if(!alcMakeContextCurrent(context))
	{
		alcDestroyContext(context);
		alcCloseDevice(device);
		throw std::runtime_error("Failed to make audio context current");
	}

	std::cout << "Initialized OpenAL: " << std::endl;
	std::cout << "  Renderer: " << alGetString(AL_RENDERER) << std::endl;
	std::cout << "  Vendor: " << alGetString(AL_VENDOR) << std::endl;
	std::cout << "  Version: " << alGetString(AL_VERSION) << std::endl;
	std::cout << "  Device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;
}

