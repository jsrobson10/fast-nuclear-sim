
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

static GLMesh StaticMeshData;
static widget::Clock WidgetClock;

void UI::init()
{
	Mesh m;

	unsigned int handle = Texture::handle_white;
	const unsigned int indices[] = {0, 1, 3, 0, 3, 2};
	const Arrays::vertex vertices[] = {
		Arrays::vertex(handle, {0, 0}, {-1, -1, 0, 1}, {0, 0, -1}),
		Arrays::vertex(handle, {0, 1}, {-1,  1, 0, 1}, {0, 0, -1}),
		Arrays::vertex(handle, {1, 0}, { 1, -1, 0, 1}, {0, 0, -1}),
		Arrays::vertex(handle, {1, 1}, { 1,  1, 0, 1}, {0, 0, -1}),
	};

	m.set_indices(indices, 6);
	m.set_vertices(vertices, 4);

	StaticMeshData.bind();
	StaticMeshData.set(m, GL_STATIC_DRAW);
	StaticMeshData.colour_matrix = glm::scale(glm::mat4(1), glm::vec3(1) * 0.75f);
}

void UI::update(double dt)
{
	WidgetClock.update(dt);
}

void UI::render()
{
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::vec2 wsize(Resize::get_size() / 2);
	
	glm::mat4 mat_projection = glm::mat4(1);
	glm::mat4 mat_camera = glm::scale(glm::mat4(1), glm::vec3(1.0f / wsize * glm::vec2(1, -1), -1));
	glUniformMatrix4fv(Shader::gl_projection, 1, false, &mat_projection[0][0]);
	glUniformMatrix4fv(Shader::gl_camera, 1, false, &mat_camera[0][0]);

	StaticMeshData.bind();
	StaticMeshData.uniform();
	StaticMeshData.render();

	WidgetClock.render();
}

