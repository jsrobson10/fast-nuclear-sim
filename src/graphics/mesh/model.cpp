
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>

#include "mesh.hpp"
#include "arrays.hpp"
#include "texture.hpp"

using namespace sim::graphics;

struct proc_state
{
	unsigned int offset = 0;

	std::string base;
	std::vector<arrays::vertex> vertices;
	std::vector<unsigned int> indices;
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

static void proc_mesh(proc_state& state, aiMesh* mesh, const aiScene* scene)
{
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	unsigned int texid = proc_texture(state, material, aiTextureType_DIFFUSE);
	unsigned int offset = state.offset;
	
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		arrays::vertex vertex;
		
		auto [x, y, z] = mesh->mVertices[i];
		vertex.pos = {y, x, -z};
		vertex.texid = texid;

		if(mesh->HasNormals())
		{
			auto [x, y, z] = mesh->mNormals[i];
			vertex.normal = {y, x, -z};
		}

		if(mesh->mTextureCoords[0])
		{
			auto [x, y, z] = mesh->mTextureCoords[0][i];
			vertex.texpos = {x, y};
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

void mesh::load_model(std::string base, std::string filename)
{
	proc_state state {.base = base};
	std::string path = base + "/" + filename;
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
	proc_node(state, scene->mRootNode, scene);

	set_vertices(&state.vertices[0], state.vertices.size(), GL_STATIC_DRAW);
	set_indices(&state.indices[0], state.indices.size(), GL_STATIC_DRAW);
}

