#pragma once
#include "config.h"
#include "interface_system.h"
#include "render_system.h"
#include "input_system.h"
#include "surface_system.h"

class App
{
public:
	App(uint winX, uint winY);
	~App();
	void run();

private:
	GLFWwindow* window;
	std::unique_ptr<InputSystem> input_system;
	std::unique_ptr<RenderSystem> render_system;
	std::shared_ptr<InterfaceSystem> interface_system;
	std::shared_ptr<Shader> default_shader, light_shader, tex_shader;
	std::shared_ptr<SurfaceSystem> surface_system;
	std::shared_ptr<LightSource> light_source;

	void set_up_glfw(uint winX, uint winY);
	void set_up_imgui();
	void set_up_opengl();
	void make_shaders();
	void make_systems();
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

