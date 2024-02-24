
#include "ui.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_transform.hpp>

#include "mesh/mesh.hpp"
#include "mesh/glmesh.hpp"
#include "mesh/arrays.hpp"
#include "mesh/font.hpp"
#include "mesh/texture.hpp"
#include "resize.hpp"
#include "shader.hpp"

#include "widget/clock.hpp"

using namespace Sim::Graphics;

static GLMesh gm_ui;
static GLMesh gm_dynamic_slow[2];

static Widget::Clock w_clock;

static int gm_dynamic_slow_at = 0;

void UI::init()
{
	Mesh m;

	unsigned int handle = Texture::handle_white;
	const unsigned int indices[] = {0, 1, 3, 0, 3, 2};
	const Arrays::Vertex vertices[] = {
		{.texid=handle, .texpos={0, 0}, .pos={-1, -1, 0, 1}, .normal={0, 0, -1}, .colour={1, 1, 1, 1}, .material={0, 0, 1}},
		{.texid=handle, .texpos={0, 1}, .pos={-1,  1, 0, 1}, .normal={0, 0, -1}, .colour={1, 1, 1, 1}, .material={0, 0, 1}},
		{.texid=handle, .texpos={1, 0}, .pos={ 1, -1, 0, 1}, .normal={0, 0, -1}, .colour={1, 1, 1, 1}, .material={0, 0, 1}},
		{.texid=handle, .texpos={1, 1}, .pos={ 1,  1, 0, 1}, .normal={0, 0, -1}, .colour={1, 1, 1, 1}, .material={0, 0, 1}},
	};

	m.set_indices(indices, 6);
	m.set_vertices(vertices, 4);

	gm_ui.bind();
	gm_ui.set(m, GL_STATIC_DRAW);
}

void UI::update(double dt)
{
	w_clock.update(dt);
}

void UI::update_slow()
{
	Mesh mesh;

	w_clock.remesh_slow(mesh);

	gm_dynamic_slow[gm_dynamic_slow_at].bind();
	gm_dynamic_slow[gm_dynamic_slow_at].set(mesh, GL_DYNAMIC_DRAW);
	gm_dynamic_slow_at = (gm_dynamic_slow_at + 1) % 2;
}

void UI::render()
{
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::vec2 wsize(Resize::get_size() / 2);
	
	glm::mat4 mat_projection = glm::mat4(1);
	glm::mat4 mat_camera = glm::scale(glm::mat4(1), glm::vec3(1.0f / wsize * glm::vec2(1, -1), -1));
	glUniformMatrix4fv(Shader::MAIN["projection"], 1, false, &mat_projection[0][0]);
	glUniformMatrix4fv(Shader::MAIN["camera"], 1, false, &mat_camera[0][0]);

	gm_ui.bind();
	gm_ui.uniform();
	gm_ui.render();
	
	gm_dynamic_slow[gm_dynamic_slow_at].bind();
	gm_dynamic_slow[gm_dynamic_slow_at].uniform();
	gm_dynamic_slow[gm_dynamic_slow_at].render();

	w_clock.render();
}

