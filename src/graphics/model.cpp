
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include "model.hpp"
#include "arrays.hpp"

using namespace sim::graphics;

model::model()
{
	
}

model::~model()
{
	if(vbo) glDeleteBuffers(1, &vbo);
	if(ebo) glDeleteBuffers(1, &ebo);
	if(vao) glDeleteVertexArrays(1, &vao);
}

void model::alloc()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	arrays::vertex_attrib_pointers();
}

void model::load_vbo(const arrays::vertex* data, size_t size, int mode)
{
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(data[0]), data, mode);
}

void model::load_ebo(const unsigned int* data, size_t size, int mode)
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(data[0]), data, mode);
	this->size = size;
}

struct proc_state
{
	size_t at = 0;
	std::vector<arrays::vertex> vertices;
	std::vector<unsigned int> indices;
};

static void proc_mesh(proc_state& state, aiMesh* mesh, const aiScene* scene)
{
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		arrays::vertex vertex;

		vertex.pos = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

		state.vertices.push_back(vertex);
	}

	unsigned int at = state.at;

	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		unsigned int j;
		
		for(j = 0; j < face.mNumIndices; j++)
		{
			state.indices.push_back(face.mIndices[j] + state.at);
		}

		at += j;
	}

	state.at = at;
}

static void proc_node(proc_state& state, aiNode* node, const aiScene* scene)
{
	for(size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		proc_mesh(state, mesh, scene);
	}

	for(size_t i = 0; i < node->mNumChildren; i++)
	{
		proc_node(state, node->mChildren[i], scene);
	}
}

void model::load(const char* path)
{
	proc_state state;
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	proc_node(state, scene->mRootNode, scene);

	load_vbo(&state.vertices[0], state.vertices.size(), GL_STATIC_DRAW);
	load_ebo(&state.indices[0], state.indices.size(), GL_STATIC_DRAW);
}

void model::bind()
{
	glBindVertexArray(vao);
}

void model::render()
{
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}

