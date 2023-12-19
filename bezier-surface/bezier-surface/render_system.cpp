#include "render_system.h"

RenderSystem::RenderSystem(
	std::shared_ptr<InterfaceSystem> interface_system, 
	std::shared_ptr<SurfaceSystem> surface_system,
	std::shared_ptr<LightSource> light_source
) : interface_system(interface_system), surface_system(surface_system), light_source(light_source)
{
}

void RenderSystem::update()
{
	glClearColor(0.2f, 0.1f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	
	surface_system->draw();
	light_source->draw();

	interface_system->draw_interface(); // shall be last
	
}