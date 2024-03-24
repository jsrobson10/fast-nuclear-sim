
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <cmath>

#include "cctv.hpp"
#include "../shader.hpp"
#include "../window.hpp"
#include "../camera.hpp"
#include "../input/focus.hpp"
#include "../data/texture.hpp"
#include "../data/font.hpp"
#include "../../system.hpp"
#include "../../util/math.hpp"
#include "../../util/streams.hpp"
#include "../statebuffer.hpp"

#define HEIGHT 512

using namespace Sim::Graphics::Monitor;
using namespace Sim::Graphics::Data;
using namespace Sim::Graphics;
using namespace Sim;

class FocusCCTV : public Focus::FocusType
{
	CCTV* parent;

	int zoom = 0;
	int rot_pitch = 0;
	int rot_yaw = 0;

public:

	FocusCCTV(CCTV* parent) : parent(parent)
	{
	}

	virtual void update(double dt)
	{
		if(rot_pitch || rot_yaw)
		{
			parent->rotate(dt, rot_pitch, -rot_yaw);
		}

		if(zoom)
		{
			Data::Camera& active = parent->cameras[parent->camera_at];
			active.zoom = Util::Math::clamp(active.zoom - zoom * dt * 0.5f, 0.25, 1);
		}
	}

	virtual void on_keypress(int key, int sc, int action, int mods)
	{
		if(action == GLFW_PRESS)
		{
			switch(key)
			{
			case GLFW_KEY_KP_1:
				parent->camera_at = (parent->camera_at + parent->cameras.size() - 1) % parent->cameras.size();
				break;
			case GLFW_KEY_KP_2:
				parent->powered = !parent->powered;
				break;
			case GLFW_KEY_KP_3:
				parent->camera_at = (parent->camera_at + 1) % parent->cameras.size();
				break;
			case GLFW_KEY_KP_4:
				rot_yaw += 1;
				break;
			case GLFW_KEY_KP_5:
				rot_pitch -= 1;
				break;
			case GLFW_KEY_KP_6:
				rot_yaw -= 1;
				break;
			case GLFW_KEY_KP_7:
				zoom += 1;
				break;
			case GLFW_KEY_KP_8:
				rot_pitch += 1;
				break;
			case GLFW_KEY_KP_9:
				zoom -= 1;
				break;
			}
		}

		else if(action == GLFW_RELEASE)
		{
			switch(key)
			{
			case GLFW_KEY_KP_4:
				rot_yaw -= 1;
				break;
			case GLFW_KEY_KP_5:
				rot_pitch += 1;
				break;
			case GLFW_KEY_KP_6:
				rot_yaw += 1;
				break;
			case GLFW_KEY_KP_7:
				zoom -= 1;
				break;
			case GLFW_KEY_KP_8:
				rot_pitch -= 1;
				break;
			case GLFW_KEY_KP_9:
				zoom += 1;
				break;
			}
		}
	}
};

CCTV::CCTV(Model& model)
	: Data::MeshGen("cctv")
	, height(HEIGHT)
	, width(HEIGHT * 16 / 9)
	, cameras(model.cameras)
{
	m_buttons[0] = model.load("click_cctv_numpad_1");
	m_buttons[1] = model.load("click_cctv_numpad_2");
	m_buttons[2] = model.load("click_cctv_numpad_3");
	m_buttons[3] = model.load("click_cctv_numpad_4");
	m_buttons[4] = model.load("click_cctv_numpad_5");
	m_buttons[5] = model.load("click_cctv_numpad_6");
	m_buttons[6] = model.load("click_cctv_numpad_7");
	m_buttons[7] = model.load("click_cctv_numpad_8");
	m_buttons[8] = model.load("click_cctv_numpad_9");

	glGenFramebuffers(2, fbos);
	glGenTextures(2, textures);
	glGenRenderbuffers(1, &rbo_depth);

	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	for(int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbos[i]);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[i], 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
	}

	mat = model.load_matrix("translation_monitor_1");
	m_screen.vertices = {
		{.texpos={0, 1}, .pos={0, 0, 0}, .transform_id=0, .material={0, 0, 1}},
		{.texpos={0, 0}, .pos={0, 1, 0}, .transform_id=0, .material={0, 0, 1}},
		{.texpos={1, 1}, .pos={1, 0, 0}, .transform_id=0, .material={0, 0, 1}},
		{.texpos={1, 0}, .pos={1, 1, 0}, .transform_id=0, .material={0, 0, 1}},
	};
	m_screen.indices = {0, 1, 3, 0, 3, 2};
	m_screen.transforms = {mat};
	m_screen.set_baked();

	gm_screen.bind();
	gm_screen.set(m_screen, GL_STATIC_DRAW);

	Shader::CCTV.use();
	glUniform1i(Shader::CCTV["screen"], 0);
	Shader::MAIN.use();
}

CCTV::~CCTV()
{
	if(fbos[0]) glDeleteFramebuffers(2, fbos);
	if(textures[0]) glDeleteTextures(2, textures);
	if(rbo_depth) glDeleteRenderbuffers(1, &rbo_depth);
}

CCTV::CCTV(CCTV&& o)
	: Data::MeshGen("cctv")
	, width(o.width)
	, height(o.height)
	, cameras(std::move(o.cameras))
	, gm_screen(std::move(o.gm_screen))
	, m_screen(std::move(o.m_screen))
	, m_buttons(std::move(o.m_buttons))
	, powered(o.powered)
{
	fbos[0] = o.fbos[0];
	fbos[1] = o.fbos[1];
	textures[0] = o.textures[0];
	textures[1] = o.textures[1];
	rbo_depth = o.rbo_depth;

	o.fbos[0] = 0;
	o.textures[0] = 0;
	o.rbo_depth = 0;
}

void CCTV::rotate(double dt, float pitch, float yaw)
{
	Data::Camera& active = cameras[camera_at];
	float m = float(M_PI) * dt * 0.5f * active.zoom;

	active.pitch = Util::Math::clamp(active.pitch + pitch * m, -M_PI / 4, M_PI / 4);
	active.yaw = Util::Math::clamp(active.yaw + yaw * m, -M_PI / 4, M_PI / 4);
}

void CCTV::remesh_slow(Data::Mesh& rmesh)
{
	if(!powered)
	{
		return;
	}
	
	const Data::Camera& active = cameras[camera_at];
	std::stringstream ss;
	ss << " ";

	for(int i = 0; i < cameras.size(); i++)
	{
		if(i == camera_at)
		{
			ss << "[" << cameras[i].name << "] ";
		}

		else
		{
			ss << " " << cameras[i].name << "  ";
		}
	}

	Data::Fonts::MONO.load_text(rmesh, ss.str(), {.size=0.02, .align={0.5, 0}, .mat=glm::translate(mat, {0.5, 0.95, 0}), .bake=true});
	
	char zoom_chars[] = "          ";
	zoom_chars[(int)std::round(Util::Math::ramp(active.zoom, 1, 0.25, 0, 9))] = '#';

	ss.str("");
	ss << "Zoom: [" << zoom_chars << "]";

	Data::Fonts::MONO.load_text(rmesh, ss.str(), {.size=0.02, .align={0, 1}, .mat=glm::translate(mat, {0.0125, 0.05, 0}), .bake=true});
}

void CCTV::update(double dt)
{
	Data::Camera& active = cameras[camera_at];

	if(m_screen.check_focus())
		Focus::set(std::make_unique<FocusCCTV>(this));
	if(m_buttons[0].check_focus_hold(Focus::Trigger::INTERFACE))
		active.zoom = Util::Math::clamp(active.zoom - dt * 0.5f, 0.25, 1);
	if(m_buttons[1].check_focus_hold(Focus::Trigger::INTERFACE))
		rotate(dt, 1, 0);
	if(m_buttons[2].check_focus_hold(Focus::Trigger::INTERFACE))
		active.zoom = Util::Math::clamp(active.zoom + dt * 0.5f, 0.25, 1);
	if(m_buttons[3].check_focus_hold(Focus::Trigger::INTERFACE))
		rotate(dt, 0, -1);
	if(m_buttons[4].check_focus_hold(Focus::Trigger::INTERFACE))
		rotate(dt, -1, 0);
	if(m_buttons[5].check_focus_hold(Focus::Trigger::INTERFACE))
		rotate(dt, 0, 1);
	if(m_buttons[6].check_focus(Focus::Trigger::INTERFACE))
		camera_at = (camera_at + cameras.size() - 1) % cameras.size();
	if(m_buttons[7].check_focus(Focus::Trigger::INTERFACE))
		powered = !powered;
	if(m_buttons[8].check_focus(Focus::Trigger::INTERFACE))
		camera_at = (camera_at + 1) % cameras.size();
}

void CCTV::render_view()
{
	if(!powered)
		return;

	Data::Camera& active = cameras[camera_at];

	glm::mat4 rot = glm::mat4(1);
	glm::vec3 right = glm::normalize(glm::cross(active.look, active.up));
	rot = glm::rotate(rot, -active.yaw, active.up);
	rot = glm::rotate(rot, active.pitch, right);

	glm::mat4 view = glm::lookAt(active.pos, active.pos + glm::mat3(rot) * active.look, active.up);
	glm::mat4 proj = glm::perspective(active.fov * active.zoom, (float)width / height, 0.1f, 100.0f);
	glm::vec3 brightness = glm::vec3(System::active->grid.get_light_intensity());

	glBindFramebuffer(GL_FRAMEBUFFER, fbos[(buff_at + 1) % 2]);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CCW);

	StateBuffer::set({view, proj});
	glUniform3fv(Shader::MAIN["camera_pos"], 1, &active.pos[0]);

	Window::bind_scene_ssbo();
	Window::render_scene();
	Window::render_dynamic();
	Window::render_player();
	render_screen();

	buff_at = (buff_at + 1) % 2;
}

void CCTV::render_screen()
{
	if(!powered)
		return;

	Shader::CCTV.use();
	glBindTexture(GL_TEXTURE_2D, textures[buff_at]);
	glBlendFunc(GL_SRC_COLOR, GL_ONE);

	gm_screen.bind();
	gm_screen.render();
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Shader::MAIN.use();
}

