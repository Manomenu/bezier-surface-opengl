#pragma once
#include "config.h"
#include "interface_system.h"
#include "surface_system.h"
#include "light_source.h"

class RenderSystem
{
public:
	RenderSystem(
		std::shared_ptr<InterfaceSystem> interface_system,
		std::shared_ptr<SurfaceSystem> surface_system,
		std::shared_ptr<LightSource> light_source
	);
	void update();

private:
	std::shared_ptr<InterfaceSystem> interface_system;
	std::shared_ptr<SurfaceSystem> surface_system;
	std::shared_ptr<LightSource> light_source;
};

