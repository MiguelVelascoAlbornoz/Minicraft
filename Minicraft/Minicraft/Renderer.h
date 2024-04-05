#pragma once

#include "Camera.h"
#include <GLFW/glfw3.h>

class Renderer
{
public:
	~Renderer();

	bool initRenderer(bool fullscreen);
	static void errorCallback(int error, const char* description);
	bool glLogCall(const char* function, const char* file, int line);
	void fill(Shader* shader, vec3* color);
	void background( GLfloat r, GLfloat g, GLfloat b);
	void toggleFullscreen();
	void update();
	void fill(Shader* shader, GLfloat r, GLfloat g, GLfloat b);
	void drawRect(float px, float py, float width, float height,float z);
	const bool& getIsFullscreen() { return &isFullscreen; };
	const vec2& getWindowSize() { return windowSize; };
	GLFWwindow* window = nullptr;
private:

	bool isFullscreen = true;
	vec2 windowSize = vec2();
	vec3 color = vec3(0.0f,0.0f,0.0f);

};

