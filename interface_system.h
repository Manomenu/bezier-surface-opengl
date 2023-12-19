#pragma once
#include "config.h"
#include "surface_system.h"
#include "light_source.h"

class InterfaceSystem
{
public:
	InterfaceSystem(std::shared_ptr<SurfaceSystem> surface_p);
	~InterfaceSystem();
	void draw_interface();
	void update_surface_triangles_density();
	void update_surface_reference_point();
	bool is_reference_points_configurating() { return reference_points_config; }
	void update_uniforms();
	void update_light_movement();
	glm::vec3 get_light_color();
	int get_fill_mode() { return fill_mode; }
	void update_NormalMap_status();
	bool filling = 1;
	float alfa = M_PI / 4;
	float beta = M_PI / 4;
	bool grid = 0;

private:
	std::shared_ptr<SurfaceSystem> surface_p;
	bool show_window = true;
	bool openFile(std::string& sSelectedFile, std::string& sFilePath);
	bool isNormalMapOn = false, prev_isNormalMapOn = false;
	bool light_config = false;
	bool reference_points_config = false;
	bool prev_reference_points_config = false;
	int fill_mode = 1;
	int powFun = 0;
	int triangles_density_value = 11;
	int prev_triangles_density_value = DEFAULT_TRIANGLES_DENSITY;
	float point_height[16];
	float ks = 0.5;
	float kd = 0.5;
	int m = 20;
	glm::vec3 object_color = { 1.0f, 1.0f, 1.0f };

	bool light_moves = true;
	glm::vec3 light_color = { 1.0f, 1.0f, 1.0f };
};