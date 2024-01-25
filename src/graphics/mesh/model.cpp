
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/matrix.hpp>

#include <unordered_map>
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
	std::unordered_map<const aiTexture*, unsigned int> handles;
};

static unsigned int proc_texture(const proc_state& state, aiMaterial* mat, const aiScene* scene)
{
	for(int i = 0; i < 0x0d; i++)
	{
		aiTextureType type = (aiTextureType)i;

		if(mat->GetTextureCount(type) == 0)
		{
			continue;
		}

		aiString str;
		mat->GetTexture(type, 0, &str);
		
		const aiTexture* tex = scene->GetEmbeddedTexture(str.C_Str());

		if(tex != nullptr)
		{
			unsigned int handle = state.handles.find(tex)->second;
			std::cout << "Using preloaded texture: " << tex->mFilename.C_Str() << "\n";
			return handle;
		}

		std::string filename(str.C_Str());
		std::replace(filename.begin(), filename.end(), '\\', '/');

		return texture::load(state.base + "/" + filename);
	}

	return 0;
}

static void proc_mesh(proc_state& state, glm::mat4 mat, aiMesh* mesh, const aiScene* scene)
{
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	unsigned int texid = proc_texture(state, material, scene);
	unsigned int offset = state.offset;
	
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		arrays::vertex vertex;
		
		auto [x, y, z] = mesh->mVertices[i];
		vertex.pos = glm::vec4(x, y, z, 1) * mat;
		vertex.texid = texid;

		if(mesh->HasNormals())
		{
			auto [x, y, z] = mesh->mNormals[i];
			vertex.normal = glm::vec3(x, y, z) * glm::mat3(mat);
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

static void proc_node(proc_state& state, glm::mat4 mat, aiNode* node, const aiScene* scene)
{
	auto m = node->mTransformation;
	mat = glm::mat4(
		m.a1, m.a2, m.a3, m.a4,
		m.b1, m.b2, m.b3, m.b4,
		m.c1, m.c2, m.c3, m.c4,
		m.d1, m.d2, m.d3, m.d4
	) * mat;
	
	for(size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		proc_mesh(state, mat, mesh, scene);
	}

	for(size_t i = 0; i < node->mNumChildren; i++)
	{
		proc_node(state, mat, node->mChildren[i], scene);
	}
}

static unsigned int proc_embedded_texture(aiTexture* tex)
{
	std::cout << "Loading embedded data: " << tex->mFilename.C_Str() << "\n";
	
	if(tex->mHeight == 0)
	{
		return texture::load_mem((unsigned char*)tex->pcData, tex->mWidth);
	}

	// swizzle each pixel to get RGBA
	for(int i = 0; i < tex->mWidth * tex->mHeight; i++)
	{
		aiTexel t = tex->pcData[i];
		tex->pcData[i] = {t.r, t.g, t.b, t.a};
	}

	return texture::load_mem((unsigned char*)tex->pcData, tex->mWidth, tex->mHeight, 4);
}

void mesh::load_model(std::string base, std::string filename)
{
	proc_state state {.base = base};
	std::string path = base + "/" + filename;
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if(scene == nullptr)
	{
		std::cerr << "AssImp: Error loading model\n";
		return;
	}

	for(int i = 0; i < scene->mNumTextures; i++)
	{
		aiTexture* tex = scene->mTextures[i];
		unsigned int handle = proc_embedded_texture(tex);
		state.handles[tex] = handle;
	}

	proc_node(state, glm::mat4(1), scene->mRootNode, scene);

	set_vertices(&state.vertices[0], state.vertices.size(), GL_STATIC_DRAW);
	set_indices(&state.indices[0], state.indices.size(), GL_STATIC_DRAW);
}

