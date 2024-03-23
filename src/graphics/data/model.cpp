
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>

#include <unordered_map>
#include <iostream>
#include <sstream>
#include <vector>

#include "mesh.hpp"
#include "arrays.hpp"
#include "texture.hpp"
#include "model.hpp"
#include "../../util/streams.hpp"

using namespace Sim::Graphics::Data;

struct ProcState
{
	unsigned int offset = 0;

	std::string base;
	std::vector<Arrays::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::mat4> transforms;
	const std::vector<Model::Material>& materials;
};

static void proc_mesh(ProcState& state, aiMesh* mesh, const aiScene* scene)
{
	Model::Material mat = state.materials[mesh->mMaterialIndex];
	unsigned int offset = state.offset;
	
	if(!mesh->HasNormals())
	{
		throw std::runtime_error("Mesh has no normals");
	}
	
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Arrays::Vertex vertex;
		
		auto [x, y, z] = mesh->mVertices[i];
		vertex.transform_id = state.transforms.size();
		vertex.pos = glm::vec3(x, y, z);
		vertex.colour = mat.colour;
		vertex.tex_diffuse = mat.diffuse;
		vertex.tex_normal = mat.normal;
		vertex.material = {mat.roughness, mat.metalness, mat.luminance};
		
		if(mesh->mTextureCoords[0])
		{
			auto [x, y, z] = mesh->mTextureCoords[0][i];
			vertex.texpos = {x, y};
		}
		
		if(mesh->HasTangentsAndBitangents())
		{
			auto [x1, y1, z1] = mesh->mTangents[i];
			auto [x2, y2, z2] = mesh->mBitangents[i];
			auto [x3, y3, z3] = mesh->mNormals[i];

			vertex.tbn = {
				{x1, y1, z1},
				{x2, y2, z2},
				{x3, y3, z3},
			};
		}

		else
		{
			auto [x, y, z] = mesh->mNormals[i];

			glm::vec3 normal = {x, y, z};
			glm::vec3 tangent = glm::normalize(glm::cross(normal, {-y, z, x}));
			glm::vec3 bitangent = glm::normalize(glm::cross(normal, tangent));

			vertex.tbn = {tangent, bitangent, normal};
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
		m.a1, m.b1, m.c1, m.d1,
		m.a2, m.b2, m.c2, m.d2,
		m.a3, m.b3, m.c3, m.d3,
		m.a4, m.b4, m.c4, m.d4
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

static void proc_node(ProcState& state, glm::mat4 mat, aiNode* node, const aiScene* scene, const std::string& name, bool found = false)
{
	mat = mat * convert_mat(node->mTransformation);
	
	if(!found && name == node->mName.C_Str())
	{
		found = true;
	}

	if(found)
	{
		int count = 0;

		for(size_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			
			if(mesh->mNumVertices == 0 || mesh->mNumFaces == 0)
			{
				continue;
			}

			proc_mesh(state, mesh, scene);
			count++;
		}
	
		if(count > 0)
		{
			state.transforms.push_back(mat);
		}
	}

	for(size_t i = 0; i < node->mNumChildren; i++)
	{
		proc_node(state, mat, node->mChildren[i], scene, name, found);
	}
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

glm::mat4 Model::load_matrix(const char* name) const
{
	return get_transforms(scene->mRootNode->FindNode(name));
}

static uint32_t proc_texture(
		const std::string& path_base,
		aiMaterial* mat,
		const aiScene* scene,
		aiTextureType type,
		int index,
		uint64_t handle_fail=0)
{
	aiString str;
	mat->GetTexture(type, index, &str);

	if(str.C_Str()[0] == '\0')
	{
		return handle_fail;
	}
	
	std::string filename(str.C_Str());
	std::replace(filename.begin(), filename.end(), '\\', '/');

	return Texture::load(path_base + "/" + filename);
}

Model::Model(std::string base, std::string filename) : base(base)
{
	std::string path = base + "/" + filename;
	scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	materials.reserve(scene->mNumMaterials);
	
	for(int i = 0; i < scene->mNumLights; i++)
	{
		aiLight* light = scene->mLights[i];
		glm::mat4 mat = load_matrix(light->mName.C_Str());

		auto [x, y, z] = light->mPosition;
		auto [r, g, b] = light->mColorDiffuse;

		glm::vec4 pos = mat * glm::vec4(x, y, z, 1);

		lights.push_back({
			glm::vec3(pos),
			{r, g, b},
		});
	}

	for(int i = 0; i < scene->mNumCameras; i++)
	{
		aiCamera* camera = scene->mCameras[i];
		glm::mat4 mat = load_matrix(camera->mName.C_Str());

		auto [x, y, z] = camera->mPosition;
		auto [dx, dy, dz] = camera->mLookAt;
		auto [ux, uy, uz] = camera->mUp;

		glm::vec4 pos = mat * glm::vec4(x, y, z, 1);
		glm::vec3 look = glm::normalize(glm::mat3(mat) * glm::vec3(dx, dy, dz));
		glm::vec3 up = glm::normalize(glm::mat3(mat) * glm::vec3(ux, uy, uz));

		cameras.push_back(Camera {
			.name={camera->mName.C_Str()},
			.pos=glm::vec3(pos),
			.look=look,
			.up=up,
			.fov=camera->mHorizontalFOV
		});
	}

	for(int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* ai_mat = scene->mMaterials[i];

		glm::vec3 matv(0);
		aiColor4D ai_cb;
		aiColor4D ai_em;
	
		ai_mat->Get(AI_MATKEY_COLOR_EMISSIVE, ai_em);
		ai_mat->Get(AI_MATKEY_BASE_COLOR, ai_cb);

		glm::vec4 cb = {ai_cb[0], ai_cb[1], ai_cb[2], ai_cb[3]};
		glm::vec4 em = {ai_em[0], ai_em[1], ai_em[2], ai_em[3]};

		if(em.x > 0 || em.y > 0 || em.z > 0)
		{
			cb = em;
		}

		ai_mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, matv[0]);
		ai_mat->Get(AI_MATKEY_METALLIC_FACTOR, matv[1]);
		ai_mat->Get(AI_MATKEY_EMISSIVE_INTENSITY, matv[2]);

		uint32_t handle_diffuse = proc_texture(base, ai_mat, scene, aiTextureType_BASE_COLOR, 0, Texture::handle_white);
		uint32_t handle_normal = proc_texture(base, ai_mat, scene, aiTextureType_NORMALS, 0, Texture::handle_normal);

		Material mat = {
			.diffuse = handle_diffuse,
			.normal = handle_normal,
			.colour = cb,
			.roughness = matv[0],
			.metalness = matv[1],
			.luminance = matv[2],
		};

		materials.push_back(mat);
	}
}

Mesh Model::load(const char* name, glm::mat4 mat) const
{
	Mesh mesh;
	ProcState state {
		.base = base,
		.materials = materials,
	};

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

