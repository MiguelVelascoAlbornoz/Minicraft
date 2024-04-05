#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Renderer.h"
using namespace glm;
class Face
{
public:
	Face(GLfloat* vertices);
	GLuint VAO;

	~Face();
	bool shouldDestroy = true;
	void draw();
};

