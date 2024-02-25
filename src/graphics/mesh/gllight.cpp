
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gllight.hpp"
#include "../shader.hpp"
#include "../window.hpp"
#include "texture.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

using namespace Sim::Graphics;

static glm::mat4 shadow_mats[6];

GLLight::GLLight(Light light) : light(light), size(4096)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	
	for(int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, id, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	handle = glGetTextureHandleARB(id);
	glMakeTextureHandleResidentARB(handle);
}

GLLight::GLLight(GLLight&& o) : light(o.light), size(o.size)
{
	id = o.id;
	handle = o.handle;
	fbo = o.fbo;

	o.id = 0;
	o.handle = 0;
	o.fbo = 0;
}

GLLight::~GLLight()
{
	std::cout << "Destructor called for " << fbo << ":" << id << ":" << handle << "\n";

	if(fbo)
		glDeleteFramebuffers(1, &fbo);
	if(id)
		glDeleteTextures(1, &id);
}

void GLLight::render()
{
	glm::mat4 camera_mat = glm::translate(glm::mat4(1), -light.pos);
	glUniformMatrix4fv(Shader::LIGHT["camera"], 1, false, &camera_mat[0][0]);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, size, size);
	Window::render_scene();
}

void GLLight::init()
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

