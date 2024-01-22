
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include "mesh.hpp"
#include "model.hpp"
#include "arrays.hpp"

using namespace sim::graphics;

static void proc_mesh(std::vector<mesh>& meshes, aiMesh* mesh, const aiScene* scene)
{
	std::vector<arrays::vertex> vertices;
	std::vector<unsigned int> indices;
	
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		arrays::vertex vertex;

		vertex.pos = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

		if(mesh->HasNormals())
		{
			vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
		}

		vertices.push_back(vertex);
	}

	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		
		for(unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	sim::graphics::mesh m;
	m.set_vertices(&vertices[0], vertices.size(), GL_STATIC_DRAW);
	m.set_indices(&indices[0], indices.size(), GL_STATIC_DRAW);
	meshes.push_back(std::move(m));
}

static void proc_node(std::vector<mesh>& meshes, aiNode* node, const aiScene* scene)
{
	for(size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		proc_mesh(meshes, mesh, scene);
	}

	for(size_t i = 0; i < node->mNumChildren; i++)
	{
		proc_node(meshes, node->mChildren[i], scene);
	}
}

void model::load(const char* path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	proc_node(meshes, scene->mRootNode, scene);
}

void model::render() const
{
	for(const mesh& m : meshes)
	{
		m.bind();
		m.render();
	}
}

