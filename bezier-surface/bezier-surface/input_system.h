#pragma once
#include "config.h"

class InputSystem
{
public:
	InputSystem(GLFWwindow* window);
	void process_input();

private:
	GLFWwindow* window;
};

