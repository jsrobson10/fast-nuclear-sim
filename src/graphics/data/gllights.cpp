
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gllights.hpp"
#include "../shader.hpp"
#include "../window.hpp"
#include "../settings.hpp"
#include "texture.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

using namespace Sim::Graphics::Data;

static glm::mat4 shadow_mats[6];

void GLLights::init_fbo()
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, texid);
	glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_DEPTH_COMPONENT, size, size, 6 * lights.size(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texid, 0);
}

GLLights::GLLights(std::vector<Light>&& _lights) : lights(_lights)
{
	size = Settings::get_shadow_size();
	init_fbo();

	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Light) * lights.size(), lights.data(), GL_STATIC_COPY);
}

GLLights::GLLights(GLLights&& o) : lights(o.lights), size(o.size)
{
	texid = o.texid;
	fbo = o.fbo;

	o.texid = 0;
	o.fbo = 0;
}

GLLights::~GLLights()
{
	if(fbo)
		glDeleteFramebuffers(1, &fbo);
	if(texid)
		glDeleteTextures(1, &texid);
}

void GLLights::render()
{
	int size_new = Settings::get_shadow_size();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	if(size_new != size)
	{
		size = size_new;
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &texid);
		init_fbo();
	}

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, size, size);

	Window::bind_scene_ssbo();

	for(int i = 0; i < lights.size(); i++)
	{
		glUniform3fv(Shader::LIGHT["light_pos"], 1, &lights[i].pos[0]);
		glUniform1i(Shader::LIGHT["light_pass"], i);
		Window::render_scene();
	}
}

void GLLights::init()
{
	glm::mat4 shadow_proj = glm::perspective<float>(M_PI * 0.5f, 1.0f, 0.01f, 100.f);

	shadow_mats[0] = shadow_proj * glm::lookAt(glm::vec3(0), { 1, 0, 0}, {0,-1, 0});
	shadow_mats[1] = shadow_proj * glm::lookAt(glm::vec3(0), {-1, 0, 0}, {0,-1, 0});
	shadow_mats[2] = shadow_proj * glm::lookAt(glm::vec3(0), { 0, 1, 0}, {0, 0, 1});
	shadow_mats[3] = shadow_proj * glm::lookAt(glm::vec3(0), { 0,-1, 0}, {0, 0,-1});
	shadow_mats[4] = shadow_proj * glm::lookAt(glm::vec3(0), { 0, 0, 1}, {0,-1, 0});
	shadow_mats[5] = shadow_proj * glm::lookAt(glm::vec3(0), { 0, 0,-1}, {0,-1, 0});

	glUniformMatrix4fv(Shader::LIGHT["shadow_mats"], 6, false, &shadow_mats[0][0][0]);
}

