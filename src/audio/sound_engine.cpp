
#include <AL/alut.h>

#include "../graphics/camera.hpp"
#include "../util/streams.hpp"
#include "sound_engine.hpp"

#include <vector>

#include <glm/gtc/matrix_transform.hpp>

using namespace Sim::Audio;

static std::vector<int> sources_ids;
static std::vector<Source*> sources_all;
static std::vector<int> sources_free;
static std::vector<glm::vec3> sources_pos;

// show all of the sources positions, so i can see if they are in the right place
void SoundEngine::remesh_debug(Graphics::Data::Mesh& rmesh)
{
	float s = 0.025;

	Graphics::Data::Mesh::Primitive<8, 36> cube {
		.vertex_base = {
			.colour = {1, 1, 0, 1},
			.material = {0, 0, 1},
		},
		.indices = {
			0, 3, 2, 0, 1, 3,
			2, 7, 6, 2, 3, 7,
			6, 5, 4, 6, 7, 5,
			4, 1, 0, 4, 5, 1,
			0, 4, 6, 0, 6, 2,
			1, 5, 7, 1, 7, 3,
		},
		.vertices = {
			{.pos = {-s, -s, -s}},
			{.pos = {-s, -s,  s}},
			{.pos = {-s,  s, -s}},
			{.pos = {-s,  s,  s}},
			{.pos = { s, -s, -s}},
			{.pos = { s, -s,  s}},
			{.pos = { s,  s, -s}},
			{.pos = { s,  s,  s}},
		},

	};

	for(glm::vec3 pos : sources_pos)
	{
		rmesh.add(cube, glm::translate(glm::mat4(1), pos));
	}

	sources_pos.clear();
}

void process_source(Source* source, glm::mat4 view = Sim::Graphics::Camera::get_matrix(), glm::vec3 view_vel = Sim::Graphics::Camera::get_velocity())
{
	if(!source)
	{
		return;
	}

	glm::mat4 mv = view * source->mat;
	glm::vec3 vel = view_vel - glm::mat3(mv) * source->velocity;
	glm::vec3 pos;

	if(source->relative)
	{
		pos = glm::mat3(mv) * source->pos;
		sources_pos.push_back(glm::vec3(glm::translate(glm::mat4(1), glm::vec3(Sim::Graphics::Camera::get_pos())) * source->mat * glm::vec4(source->pos, 1)));
	}

	else
	{
		pos = glm::vec3(mv * glm::vec4(source->pos, 1));
		sources_pos.push_back(glm::vec3(source->mat * glm::vec4(source->pos, 1)));
	}

	alSourcef(source->id, AL_PITCH, source->pitch);
	alSourcef(source->id, AL_GAIN, source->gain * std::max(0.f, 1 - glm::length(pos) / 100));
	alSourcefv(source->id, AL_POSITION, &pos[0]);
	alSourcefv(source->id, AL_VELOCITY, &vel[0]);
}

void SoundEngine::source_process(Source* source)
{
	process_source(source);
}

void SoundEngine::update()
{
	glm::mat4 view = Sim::Graphics::Camera::get_matrix();
	glm::vec3 view_vel = Sim::Graphics::Camera::get_velocity();

	for(Source* source : sources_all)
	{
		process_source(source, view, view_vel);
	}
}

void SoundEngine::source_reregister(Source* source_old, Source* source_new)
{
	sources_all[source_old->se_id] = source_new;
}

void SoundEngine::source_register(Source* source)
{
	if(sources_free.empty())
	{
		alGenSources(1, &source->id);

		source->se_id = sources_all.size();
		sources_all.push_back(source);
		sources_ids.push_back(source->id);
	}

	else
	{
		source->se_id = sources_free.back();
		sources_free.pop_back();
		sources_all[source->se_id] = source;
		source->id = sources_ids[source->se_id];
	}
}

void SoundEngine::source_unregister(Source* source)
{
	sources_free.push_back(source->se_id);
	sources_all[source->se_id] = nullptr;
	source->se_id = -1;
	source->id = 0;
}

