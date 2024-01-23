
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>

#include "mesh.hpp"
#include "model.hpp"
#include "arrays.hpp"
#include "texture.hpp"

using namespace sim::graphics;

struct proc_state
{
	unsigned int offset = 0;

	std::string base;
	std::vector<arrays::vertex> vertices;
	std::vector<unsigned int> indices;
	glm::vec<3, double> pos;
};

static unsigned int proc_texture(const proc_state& state, aiMaterial* mat, aiTextureType type)
{
	if(mat->GetTextureCount(type) == 0)
	{
		return 0;
	}

	aiString str;
	mat->GetTexture(type, 0, &str);

	std::string filename(str.C_Str());
	std::replace(filename.begin(), filename.end(), '\\', '/');

	return texture::load(state.base + "/" + filename);
}

static void proc_mesh(proc_state& state, std::vector<mesh>& meshes, aiMesh* mesh, const aiScene* scene)
{
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	unsigned int texid = proc_texture(state, material, aiTextureType_DIFFUSE);
	unsigned int offset = state.offset;
	
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		arrays::vertex vertex;

		vertex.texid = texid;
		vertex.pos = state.pos + glm::vec<3, double>(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

		if(mesh->HasNormals())
		{
			vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
		}

		if(mesh->mTextureCoords[0])
		{
			vertex.texpos = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
			vertex.do_tex = 1;
		}

		state.vertices.push_back(vertex);
	}

	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		
		for(unsigned int j = 0; j < face.mNumIndices; j++)
		{
			state.indices.push_back(face.mIndices[j] + offset);
		}
	}

	state.offset += mesh->mNumVertices;
}

static void proc_node(proc_state& state, std::vector<mesh>& meshes, aiNode* node, const aiScene* scene)
{
	for(size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		proc_mesh(state, meshes, mesh, scene);
	}

	for(size_t i = 0; i < node->mNumChildren; i++)
	{
		proc_node(state, meshes, node->mChildren[i], scene);
	}
}

void model::load(std::string base, std::string filename, glm::vec<3, double> pos)
{
	proc_state state {.base = base, .pos = pos};
	std::string path = base + "/" + filename;
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
	proc_node(state, meshes, scene->mRootNode, scene);

	sim::graphics::mesh m;
	m.set_vertices(&state.vertices[0], state.vertices.size(), GL_STATIC_DRAW);
	m.set_indices(&state.indices[0], state.indices.size(), GL_STATIC_DRAW);
	meshes.push_back(std::move(m));
}

void model::render() const
{
	for(const mesh& m : meshes)
	{
		m.bind();
		m.render();
	}
}

