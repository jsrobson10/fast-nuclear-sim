
#include "ui.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_transform.hpp>

#include "data/mesh.hpp"
#include "data/glmesh.hpp"
#include "data/arrays.hpp"
#include "data/font.hpp"
#include "data/texture.hpp"
#include "resize.hpp"
#include "shader.hpp"

#include "widget/clock.hpp"

using namespace Sim::Graphics;

static Data::Mesh g_ui;
static Data::GLMesh gm_dynamic_slow[2];
static Widget::Clock w_clock;

static int gm_dynamic_slow_at = 0;

void UI::init()
{
	g_ui.indices = {0, 1, 3, 0, 3, 2};
	g_ui.vertices = {
		{.texpos={0, 0}, .pos={-1, -1, 0}, .material={0, 0, 1}},
		{.texpos={0, 1}, .pos={-1,  1, 0}, .material={0, 0, 1}},
		{.texpos={1, 0}, .pos={ 1, -1, 0}, .material={0, 0, 1}},
		{.texpos={1, 1}, .pos={ 1,  1, 0}, .material={0, 0, 1}},
	};
	g_ui.bake_transforms();
}

void UI::update(double dt)
{
	w_clock.update(dt);
}

void UI::update_slow()
{
	Data::Mesh mesh;

	w_clock.remesh_slow(mesh);

	mesh.add(g_ui);
	mesh.bake_transforms();

	gm_dynamic_slow[gm_dynamic_slow_at].bind();
	gm_dynamic_slow[gm_dynamic_slow_at].set(mesh, GL_STREAM_DRAW);
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
	
	gm_dynamic_slow[gm_dynamic_slow_at].bind();
	gm_dynamic_slow[gm_dynamic_slow_at].render();
}

