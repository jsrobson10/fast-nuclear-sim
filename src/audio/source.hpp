
#pragma once

#include <string>
#include <glm/matrix.hpp>

namespace Sim::Audio
{

struct Source
{
	int se_id = -1;
	unsigned int id = 0;
	bool relative = false;
	float pitch = 1;
	float gain = 1;

	glm::vec3 pos = {0, 0, 0};
	glm::vec3 velocity = {0, 0, 0};
	glm::mat4 mat = glm::mat4(1);
	
	Source(const std::string& clip_name);
	Source(unsigned int clip_id);
	Source(const Source& o) = delete;
	Source(Source&& o);
	~Source();

	void process();

	void loop();
	void play();
	void stop();
	void pause();
	void rewind();
};

};

