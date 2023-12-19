#include "App.h"


App::App(uint winX, uint winY)
{
	set_up_glfw(winX, winY);
	set_up_imgui();
	set_up_opengl();
	make_shaders();
	make_systems();
	/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
}

App::~App() 
{
	glfwTerminate();
}

void App::run()
{
    while (!glfwWindowShouldClose(window))
    {
		input_system->process_input();
		//if (!interface_system->is_reference_points_configurating())
		

		// config
		interface_system->update_NormalMap_status();
		if (interface_system->get_fill_mode() == 0)
			light_source->update(default_shader, interface_system->get_light_color());
		else if (interface_system->get_fill_mode() == 1)
			light_source->update(tex_shader, interface_system->get_light_color());
		interface_system->update_uniforms();
		interface_system->update_light_movement();
		interface_system->update_surface_triangles_density();
		interface_system->update_surface_reference_point();
		

		render_system->update();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void App::make_systems()
{
	light_source = std::make_shared<LightSource>(light_shader);
	surface_system = std::make_shared<SurfaceSystem>(default_shader, tex_shader);
	interface_system = std::make_shared<InterfaceSystem>(surface_system);
	render_system = std::make_unique<RenderSystem>(interface_system, surface_system, light_source);
	input_system = std::make_unique<InputSystem>(window);
}

void App::set_up_opengl() 
{
	glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
	//Set the rendering region to the actual screen size
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	//(left, top, width, height)
	glViewport(0, 0, w, h);
}

void App::make_shaders()
{
	default_shader = std::make_shared<Shader>("./shaders/default.vs", "./shaders/default.fs");
	tex_shader = std::make_shared<Shader>("./shaders/tex.vs", "./shaders/tex.fs");
	light_shader = std::make_shared<Shader>("./shaders/light.vs", "./shaders/light.fs");
	//picking_shader = std::make_shared<Shader>("./shaders/picking.vs", "./shaders/picking.fs");
}

void App::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

	glViewport(0, 0, width, height);
}

void App::set_up_glfw(uint winX, uint winY) 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

	window = glfwCreateWindow(winX, winY, "Figure Editor", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
	}
	glfwSwapInterval(0);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Couldn't load opengl" << std::endl;
		glfwTerminate();
	}
}

void App::set_up_imgui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          
	ImGui_ImplOpenGL3_Init("#version 330");
}

