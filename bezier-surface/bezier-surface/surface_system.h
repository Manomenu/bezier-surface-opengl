#pragma once
#include "config.h"
#include "Mesh.h"

class SurfaceSystem
{
public:
	SurfaceSystem(std::shared_ptr<Shader> shader, std::shared_ptr<Shader> texShader);
	void update_triangles_density(int density);
	void draw();
	void update();
	void update_reference_points(float points[16]);
	void update_uniforms(int fill_mode,
		glm::vec3 solid_color, glm::vec3 light_color,
		float kd, float ks, int m, float alfa, float beta);
	void update_diffuse(std::string);
	void update_normal(std::string sFilePath);
	void update_normalMap_status(bool isNormalMapOn);
	void update_powFun(int val);
	int powFun = 0;
	static bool filling;
	static bool grid;

	

private:
	std::vector<Texture> textures;
	int fill_mode = 1;
	glm::vec3 control_points[16];
	int triangles_density = DEFAULT_TRIANGLES_DENSITY;
	std::vector<Vertex> updated_vertices();
	std::vector<uint> updated_indices();
	float calculate_width(float x, float y);
	float calculate_y(float x, float y);
	Mesh _surface;
	std::shared_ptr<Shader> shader; // solid color shader
	std::shared_ptr<Shader> texShader;
	float calculate_height(float x, float y);
	int calculateNewtonSymbol(unsigned int n, unsigned int k);
	float B(int i, int n, float t);
	glm::vec3 calculate_normal_vec(float x, float y);
	uint TextureFromFile(const char* path/*, const std::string& directory*/);
	
};

