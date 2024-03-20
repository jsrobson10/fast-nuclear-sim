
#pragma once

#include <vector>
#include <array>

#include "../data/meshgen.hpp"
#include "../data/camera.hpp"
#include "../data/glmesh.hpp"

namespace Sim::Graphics::Monitor
{

class CCTV : public Data::MeshGen
{
	Data::GLMesh gm_screen;
	Data::Mesh m_screen;
	std::array<Data::Mesh, 9> m_buttons;

	unsigned int fbo;
	unsigned int texture;
	unsigned int rbo_depth;
	unsigned int handle;

	const int width;
	const int height;

public:
	
	std::vector<Data::Camera> cameras;
	int camera_at = 0;
	bool powered = false;

	CCTV(Data::Model& model);
	CCTV(const CCTV&) = delete;
	CCTV(CCTV&&);
	~CCTV();
	
	void update(double dt) override;
	void rotate(double dt, float pitch, float yaw);
	void render_view();
	void render_screen();
};

};

