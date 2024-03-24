
#include "ui.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_transform.hpp>

#include "data/mesh.hpp"
#include "data/glmesh.hpp"
#include "data/arrays.hpp"
#include "data/font.hpp"
#include "data/texture.hpp"
#include "statebuffer.hpp"
#include "resize.hpp"
#include "shader.hpp"

#include "widget/clock.hpp"

using namespace Sim::Graphics;

static Data::Mesh g_ui;
static Data::GLMesh gm_dynamic;
static Widget::Clock w_clock;

void UI::init()
{
	g_ui.indices = {0, 1, 3, 0, 3, 2};
	g_ui.vertices = {
		{.texpos={0, 0}, .pos={-1, -1, 0}, .material={0, 0, 1}},
		{.texpos={0, 1}, .pos={-1,  1, 0}, .material={0, 0, 1}},
		{.texpos={1, 0}, .pos={ 1, -1, 0}, .material={0, 0, 1}},
		{.texpos={1, 1}, .pos={ 1,  1, 0}, .material={0, 0, 1}},
	};
}

void UI::update(double dt)
{
	Data::Mesh mesh(true);
	glm::vec2 wsize(Resize::get_size() / 2);
	glm::mat4 mat_scale = glm::scale(glm::mat4(1), glm::vec3(1.f / wsize.y, 1.f / wsize.y, 1));

	if(!Focus::is_focused())
	{
		mesh.add(g_ui, mat_scale);
	}

	w_clock.update(dt);
	w_clock.remesh_ui(mesh);

	Focus::remesh_ui(mesh);

	gm_dynamic.bind();
	gm_dynamic.set(mesh, GL_STREAM_DRAW);
}

void UI::render()
{
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 mat_view = glm::scale(glm::mat4(1), glm::vec3(1.0f / Resize::get_aspect(), -1, -1));
	StateBuffer::set({mat_view, glm::mat4(1)});
	
	gm_dynamic.bind();
	gm_dynamic.render();

	Focus::render_ui();
}

