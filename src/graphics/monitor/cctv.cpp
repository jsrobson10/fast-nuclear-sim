
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
#include "../../system.hpp"
#include "../../util/math.hpp"
#include "../../util/streams.hpp"

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

	FocusCCTV(CCTV* parent)
	{
		this->parent = parent;
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
			active.zoom = Util::Math::clamp(1.f / (1.f / active.zoom - zoom * dt * 0.5f), 1.f, 4.f);
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
				rot_pitch -= 1;
				break;
			case GLFW_KEY_KP_3:
				parent->camera_at = (parent->camera_at + 1) % parent->cameras.size();
				break;
			case GLFW_KEY_KP_4:
				rot_yaw += 1;
				break;
			case GLFW_KEY_KP_5:
				parent->powered = !parent->powered;
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
			case GLFW_KEY_KP_2:
				rot_pitch += 1;
				break;
			case GLFW_KEY_KP_4:
				rot_yaw -= 1;
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
	: height(HEIGHT)
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

	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &texture);
	glGenRenderbuffers(1, &rbo_depth);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	handle = glGetTextureHandleARB(texture);
	glMakeTextureHandleResidentARB(handle);

	m_screen.vertices = {
		{.texpos={0, 1}, .pos={0, 0, 0}, .transform_id=0, .tex_diffuse=handle, .material={0, 0, 1}},
		{.texpos={0, 0}, .pos={0, 1, 0}, .transform_id=0, .tex_diffuse=handle, .material={0, 0, 1}},
		{.texpos={1, 1}, .pos={1, 0, 0}, .transform_id=0, .tex_diffuse=handle, .material={0, 0, 1}},
		{.texpos={1, 0}, .pos={1, 1, 0}, .transform_id=0, .tex_diffuse=handle, .material={0, 0, 1}},
	};
	m_screen.indices = {0, 1, 3, 0, 3, 2};
	m_screen.transforms = {model.load_matrix("translation_monitor_1")};
	m_screen.bake_transforms();

	gm_screen.bind();
	gm_screen.set(m_screen, GL_STATIC_DRAW);
}

CCTV::~CCTV()
{
	if(fbo) glDeleteFramebuffers(1, &fbo);
	if(texture) glDeleteTextures(1, &texture);
	if(rbo_depth) glDeleteRenderbuffers(1, &rbo_depth);
}

CCTV::CCTV(CCTV&& o)
	: width(o.width)
	, height(o.height)
	, cameras(std::move(o.cameras))
	, gm_screen(std::move(o.gm_screen))
	, m_screen(std::move(o.m_screen))
	, m_buttons(std::move(o.m_buttons))
	, powered(o.powered)
{
	fbo = o.fbo;
	texture = o.texture;
	rbo_depth = o.rbo_depth;
	handle = o.handle;

	o.fbo = 0;
	o.texture = 0;
	o.rbo_depth = 0;
	o.handle = 0;
}

void CCTV::rotate(double dt, float pitch, float yaw)
{
	Data::Camera& active = cameras[camera_at];
	float m = float(M_PI) * dt * 0.5f / active.zoom;

	active.pitch = Util::Math::clamp(active.pitch + pitch * m, -M_PI / 4, M_PI / 4);
	active.yaw = Util::Math::clamp(active.yaw + yaw * m, -M_PI / 4, M_PI / 4);
}

void CCTV::update(double dt)
{
	Data::Camera& active = cameras[camera_at];

	if(m_screen.check_focus())
		Focus::set(std::make_unique<FocusCCTV>(this));
	if(m_buttons[0].check_focus_hold())
		active.zoom = Util::Math::clamp(1.f / (1.f / active.zoom - dt * 0.5f), 1.f, 4.f);
	if(m_buttons[1].check_focus_hold())
		rotate(dt, 1, 0);
	if(m_buttons[2].check_focus_hold())
		active.zoom = Util::Math::clamp(1.f / (1.f / active.zoom + dt * 0.5f), 1.f, 4.f);
	if(m_buttons[3].check_focus_hold())
		rotate(dt, 0, -1);
	if(m_buttons[4].check_focus())
		powered = !powered;
	if(m_buttons[5].check_focus_hold())
		rotate(dt, 0, 1);
	if(m_buttons[6].check_focus())
		camera_at = (camera_at + cameras.size() - 1) % cameras.size();
	if(m_buttons[7].check_focus_hold())
		rotate(dt, -1, 0);
	if(m_buttons[8].check_focus())
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
	glm::mat4 proj = glm::perspective(active.fov / active.zoom, (float)width / height, 0.1f, 100.0f);
	glm::vec3 brightness = glm::vec3(System::active->grid.get_light_intensity());

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CCW);
	
	glUniformMatrix4fv(Shader::MAIN["projection"], 1, false, &proj[0][0]);
	glUniformMatrix4fv(Shader::MAIN["camera"], 1, false, &view[0][0]);
	glUniform3fv(Shader::MAIN["camera_pos"], 1, &active.pos[0]);

	Window::bind_scene_ssbo();
	Window::render_scene();
	Window::render_dynamic();
	Window::render_player();
}

void CCTV::render_screen()
{
	if(!powered)
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBlendFunc(GL_SRC_COLOR, GL_ONE);

	gm_screen.bind();
	gm_screen.render();
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

