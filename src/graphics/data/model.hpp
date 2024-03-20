
#pragma once

#include "mesh.hpp"
#include "light.hpp"
#include "camera.hpp"
#include "material.hpp"

#include <string>
#include <vector>

#include <glm/matrix.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/postprocess.h>

namespace Sim::Graphics::Data
{

class Model
{
	std::string base;
	Assimp::Importer importer;
	const aiScene* scene;

public:

	std::vector<uint32_t> textures;
	std::vector<Material> materials;
	std::vector<Camera> cameras;
	std::vector<Light> lights;

	Model(std::string base, std::string filename);
	Model(const Model&) = delete;

	Mesh load_root() const;
	Mesh load_root(glm::mat4 mat) const;
	Mesh load(const char* name) const;
	Mesh load(const char* name, glm::mat4 mat) const;
	glm::mat4 load_matrix(const char* name) const;
};

};

