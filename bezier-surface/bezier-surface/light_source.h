#pragma once
#include "config.h"
#include "Shader.h"

class LightSource
{
public:
	LightSource(std::shared_ptr<Shader> light_shader);
	void update_moving(int should_move) { is_moving = should_move; }
	void update(std::shared_ptr<Shader> shader, glm::vec3 col);
	static bool is_moving;
	void draw();

private:
	uint VAO = 1, VBO = 1;
	float buff[3];
	glm::vec3 pos = { 0.5f, 0.5f, 2.1f };
	std::shared_ptr<Shader> light_shader;
	glm::vec3 color = { 1,1,1 };
};

