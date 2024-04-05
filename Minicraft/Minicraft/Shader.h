#pragma once
#include <GL/glew.h>
#include <sstream>
#include <memory>
#include <fstream>
#include <iostream>
class Shader
{
public:
	Shader();
	void loadShader();
	bool loadFile(const char* fileName, std::stringstream& result);
	void createShader(GLenum shaderType, std::string shaderSource, GLuint& shaderProgram);
	void bind();
	void unBind();
	GLint checkShaderCompilation(GLuint shader);
	GLint checkProgramLink(GLuint program);
	~Shader();

	void uniform1i(const char* name, int value);

	void uniform4f(const char* name, GLfloat r, GLfloat g, GLfloat b, GLfloat a);

	void uniform1f(const char* name, GLfloat r);

	void uniform3f(const char* name, GLfloat r, GLfloat g, GLfloat b);

	void uniform2f(const char* name, GLfloat r, GLfloat g);

	void uniformMatrix4f(const char* name, GLsizei count, GLboolean transpose, const GLfloat* matrix);

	GLuint getUniformLocation(const char* name);

private:
	GLuint m_shaderProgram;
};

