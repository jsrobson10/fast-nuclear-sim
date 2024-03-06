
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>

#include <unordered_map>
#include <iostream>
#include <vector>

#include "mesh.hpp"
#include "arrays.hpp"
#include "texture.hpp"
#include "model.hpp"
#include "../../util/streams.hpp"

using namespace Sim::Graphics;

struct ProcState
{
	unsigned int offset = 0;

	std::string base;
	std::vector<Arrays::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::mat4> transforms;
	std::unordered_map<const aiTexture*, unsigned int> handles;
};

static unsigned int proc_texture(const ProcState& state, aiMaterial* mat, const aiScene* scene, aiTextureType type, int index)
{
	aiString str;
	mat->GetTexture(type, index, &str);

	if(str.C_Str()[0] == '\0')
	{
		return 0;
	}
	
	const aiTexture* tex = scene->GetEmbeddedTexture(str.C_Str());

	if(tex != nullptr)
	{
		unsigned int handle = state.handles.find(tex)->second;
		std::cout << "Using preloaded texture: " << tex->mFilename.C_Str() << "\n";
		return handle;
	}

	std::string filename(str.C_Str());
	std::replace(filename.begin(), filename.end(), '\\', '/');

	return Texture::load(state.base + "/" + filename);
}

static void proc_mesh(ProcState& state, aiMesh* mesh, const aiScene* scene)
{
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	aiString name;

	material->Get(AI_MATKEY_NAME, name);
/*
	std::cout << "Material " << name.C_Str() << " has " << material->mNumProperties << " properties\n";

	for(int i = 0; i < material->mNumProperties; i++)
	{
		aiMaterialProperty* prop = material->mProperties[i];

		std::cout << "  " << prop->mKey.C_Str() << ": type=" << prop->mType << " index=" << prop->mIndex << " length=" << prop->mDataLength;

		if(prop->mType == 1)
		{
			float* v = (float*)prop->mData;

			std::cout << " value=";

			switch(prop->mDataLength)
			{
			case 4:
				std::cout << v[0];
				break;
			case 8:
				std::cout << "{" << v[0] << ", " << v[1] << "}";
				break;
			case 12:
				std::cout << "{" << v[0] << ", " << v[1] << ", " << v[2] << "}";
				break;
			case 16:
				std::cout << "{" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << "}";
				break;
			default:
				std::cout << "unknown";
			}
		}
		
		std::cout << "\n";
	}*/

	glm::vec3 matv(0);
	aiColor4D ai_cb;
	aiColor4D ai_em;

	material->Get(AI_MATKEY_ROUGHNESS_FACTOR, matv[0]);
	material->Get(AI_MATKEY_METALLIC_FACTOR, matv[1]);
	material->Get(AI_MATKEY_EMISSIVE_INTENSITY, matv[2]);
	material->Get(AI_MATKEY_COLOR_EMISSIVE, ai_em);
	material->Get(AI_MATKEY_BASE_COLOR, ai_cb);

	glm::vec4 cb = {ai_cb[0], ai_cb[1], ai_cb[2], ai_cb[3]};
	glm::vec4 em = {ai_em[0], ai_em[1], ai_em[2], ai_em[3]};

	if(em.x > 0 || em.y > 0 || em.z > 0)
	{
		cb = em;
	}

	unsigned int handle = proc_texture(state, material, scene, aiTextureType_BASE_COLOR, 0);
	unsigned int offset = state.offset;

	if(!handle)
	{
		handle = proc_texture(state, material, scene, aiTextureType_DIFFUSE, 0);
	}

	if(!handle)
	{
		handle = Texture::handle_white;
	}
	
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Arrays::Vertex vertex;
		
		auto [x, y, z] = mesh->mVertices[i];
		vertex.pos = glm::vec3(x, y, z);
		vertex.transform_id = state.transforms.size();
		vertex.material = matv;
		vertex.texid = handle;
		vertex.colour = cb;

		if(mesh->HasNormals())
		{
			auto [x, y, z] = mesh->mNormals[i];
			vertex.normal = glm::vec3(x, y, z);
		}
		
		/*if(mesh->HasTangentsAndBitangents())
		{
			auto [x1, y1, z1] = mesh->mTangents[i];
			auto [x2, y2, z2] = mesh->mBitangents[i];
			vertex.tangent = {x1, y1, z1};
			vertex.bitangent = {x2, y2, z2};
		}*/

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

glm::mat4 convert_mat(aiMatrix4x4 m)
{
	return {
		m.a1, m.a2, m.a3, m.a4,
		m.b1, m.b2, m.b3, m.b4,
		m.c1, m.c2, m.c3, m.c4,
		m.d1, m.d2, m.d3, m.d4
	};
}

bool starts_with(const char* base, const char* check)
{
	while(base[0] != '\0' && check[0] != '\0')
	{
		if(base[0] != check[0])
		{
			return false;
		}

		base++;
		check++;
	}

	return (check[0] == '\0');
}

static void proc_node(ProcState& state, glm::mat4 mat, aiNode* node, const aiScene* scene, const char* search)
{
	mat = convert_mat(node->mTransformation) * mat;
	
	if(starts_with(node->mName.C_Str(), search))
	{
		for(size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			proc_mesh(state, mesh, scene);
		}

		if(node->mNumMeshes > 0)
		{
			state.transforms.push_back(glm::transpose(mat));
		}
	}

	for(size_t i = 0; i < node->mNumChildren; i++)
	{
		proc_node(state, mat, node->mChildren[i], scene, search);
	}
}

static unsigned int proc_embedded_texture(aiTexture* tex)
{
	std::cout << "Loading embedded data: " << tex->mFilename.C_Str() << "\n";
	
	if(tex->mHeight == 0)
	{
		return Texture::load_mem((unsigned char*)tex->pcData, tex->mWidth);
	}

	std::vector<glm::vec<4, unsigned char>> pixels;
	pixels.reserve(tex->mWidth * tex->mHeight);

	// convert image to get RGBA
	for(int i = 0; i < tex->mWidth * tex->mHeight; i++)
	{
		aiTexel t = tex->pcData[i];
		pixels.push_back({t.r, t.g, t.b, t.a});
	}

	return Texture::load_mem(&pixels[0][0], tex->mWidth, tex->mHeight, 4);
}

glm::mat4 get_transforms(const aiNode* node)
{
	glm::mat4 mat(1);

	while(node->mParent != nullptr)
	{
		mat = mat * convert_mat(node->mTransformation);
		node = node->mParent;
	}

	return mat;
}

glm::mat4 Model::get_matrix(const char* name) const
{
	return get_transforms(scene->mRootNode->FindNode(name));
}

Model::Model(std::string base, std::string filename) : base(base)
{
	std::string path = base + "/" + filename;
	scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

	textures.reserve(scene->mNumTextures);

	for(int i = 0; i < scene->mNumTextures; i++)
	{
		aiTexture* tex = scene->mTextures[i];
		unsigned int handle = proc_embedded_texture(tex);
		textures.push_back(handle);
	}
	
	for(int i = 0; i < scene->mNumLights; i++)
	{
		aiLight* light = scene->mLights[i];
		glm::mat4 mat = get_matrix(light->mName.C_Str());

		auto [x, y, z] = light->mPosition;
		auto [r, g, b] = light->mColorDiffuse;

		glm::vec4 pos = glm::vec4(x, y, z, 1) * mat;

		lights.push_back({
			glm::vec3(pos),
			{r, g, b},
		});
	}
}

Mesh Model::load(const char* name, glm::mat4 mat) const
{
	Mesh mesh;
	ProcState state {.base = base};
	proc_node(state, mat, scene->mRootNode, scene, name);

	mesh.vertices = std::move(state.vertices);
	mesh.indices = std::move(state.indices);
	mesh.transforms = std::move(state.transforms);

	return mesh;
}

Mesh Model::load_root(glm::mat4 mat) const
{
	return load("", mat);
}

Mesh Model::load(const char* name) const
{
	return load(name, glm::mat4(1));
}

Mesh Model::load_root() const
{
	return load("", glm::mat4(1));
}

