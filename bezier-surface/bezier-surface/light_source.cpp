#include "light_source.h"

bool LightSource::is_moving = true;

LightSource::LightSource(std::shared_ptr<Shader> _light_shader)
{
	light_shader = _light_shader;

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, &buff, GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    glBindVertexArray(0);
}

void LightSource::update(std::shared_ptr<Shader> shader, glm::vec3 color)
{
	this->color = color;
	if (is_moving)
	{
		pos.x = std::cos(float(glfwGetTime())) / 4.0f + 1/2.0f;
		pos.y = std::sin(float(glfwGetTime())) / 4.0f + 1/2.0f;
	}
	shader->use();
	shader->setVec3("LightV", pos);

	buff[0] = pos.x;
	buff[1] = pos.y;
	buff[2] = pos.z;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, &buff, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void LightSource::draw()
{
	light_shader->use();
	light_shader->setVec3("aColor", color);
	glBindVertexArray(VAO);
	glPointSize(70);
	glDrawArrays(GL_POINTS, 0, 1);
	glPointSize(1);
}

