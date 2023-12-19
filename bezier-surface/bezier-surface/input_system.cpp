#include "input_system.h"

InputSystem::InputSystem(GLFWwindow* window)
	: window(window)
{
}

void InputSystem::process_input()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}