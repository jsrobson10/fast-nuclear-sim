
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

static GLMesh s_mesh;
static Widget::Clock w_clock;

void UI::init()
{
	Mesh m;

	unsigned int handle = Texture::handle_white;
	const unsigned int indices[] = {0, 1, 3, 0, 3, 2};
	const Arrays::Vertex vertices[] = {
		Arrays::Vertex(handle, {0, 0}, {-1, -1, 0, 1}, {0, 0, -1}),
		Arrays::Vertex(handle, {0, 1}, {-1,  1, 0, 1}, {0, 0, -1}),
		Arrays::Vertex(handle, {1, 0}, { 1, -1, 0, 1}, {0, 0, -1}),
		Arrays::Vertex(handle, {1, 1}, { 1,  1, 0, 1}, {0, 0, -1}),
	};

	m.set_indices(indices, 6);
	m.set_vertices(vertices, 4);

	s_mesh.bind();
	s_mesh.set(m, GL_STATIC_DRAW);
	s_mesh.colour_matrix = glm::scale(glm::mat4(1), glm::vec3(1) * 0.75f);
}

void UI::update(double dt)
{
	w_clock.update(dt);
}

void UI::render()
{
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::vec2 wsize(Resize::get_size() / 2);
	
	glm::mat4 mat_projection = glm::mat4(1);
	glm::mat4 mat_camera = glm::scale(glm::mat4(1), glm::vec3(1.0f / wsize * glm::vec2(1, -1), -1));
	glUniformMatrix4fv(Shader::MAIN["projection"], 1, false, &mat_projection[0][0]);
	glUniformMatrix4fv(Shader::MAIN["camera"], 1, false, &mat_camera[0][0]);

	s_mesh.bind();
	s_mesh.uniform();
	s_mesh.render();

	w_clock.render();
}

