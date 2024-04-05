#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include "main.h"

#pragma once


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Simulation* simulation = (Simulation*)glfwGetWindowUserPointer(window);
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			simulation->running = false;
		case GLFW_KEY_F11:
			simulation->toggleFullscreen();
		}

	}

}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (button) {
		case 0:
			leftClick = true;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (button) {
		case 0:
			leftClick = false;
		}
	}

}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	Simulation* simulation = (Simulation*)glfwGetWindowUserPointer(window);
	if (yoffset == 1) {
		if (mouseScalling) {
			simulation->camera->setZoom(simulation->camera->getZoom() * 1.1); // Realiza un zoom in al hacer girar la rueda hacia arriba
		}
		if (mouseFoving) {
			
		}
		
	}
	else if (yoffset == -1 ) {
		if ( mouseScalling) {
			simulation->camera->setZoom(simulation->camera->getZoom() / 1.1); // Realiza un zoom out al hacer girar la rueda hacia abajo
		}
		if (mouseFoving) {
			//std::vector<std::string> command;
			//command.push_back("setFov");
			//command.push_back(std::to_string(simulation->camera->getFov() - 3));
			//Commands::setFovCommand(command, simulation->camera);
		}
	
	}

}
void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	mouseDelta.x = mousePos.x - lastMousePos.x;
	mouseDelta.y = mousePos.y - lastMousePos.y;
	lastMousePos.x = mousePos.x;
	lastMousePos.y = mousePos.y;
	mousePos.x = xpos;
	mousePos.y = ypos;

	//Move camera facing
	Camera* camera = ((Simulation*)glfwGetWindowUserPointer(window))->camera;
	vec3 facing(camera->getDirection());
	vec3 newFacing(facing.x-mouseDelta.y*ellapsedTime*camera->sensibility, facing.y-mouseDelta.x*ellapsedTime*camera->sensibility, facing.z);
	camera->setDirection(newFacing);
}
