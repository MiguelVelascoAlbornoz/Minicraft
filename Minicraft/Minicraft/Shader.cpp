#include "Shader.h"


Shader::Shader()
{
	loadShader();
}
void Shader::loadShader() {
	std::stringstream vertexSourceStream;
	std::stringstream fragmentSourceStream;

	//carga el codigo de los shaders a un streamString
	loadFile("vertex.glsl", vertexSourceStream);
	loadFile("fragment.glsl", fragmentSourceStream);


	//creamos el programa de shaders (programa final de la union de los shaders)
	m_shaderProgram = glCreateProgram();
	createShader(GL_VERTEX_SHADER, vertexSourceStream.str(), m_shaderProgram);
	createShader(GL_FRAGMENT_SHADER, fragmentSourceStream.str(), m_shaderProgram);

	glLinkProgram(m_shaderProgram);
	glUseProgram(m_shaderProgram);
	checkProgramLink(m_shaderProgram);
}
bool Shader::loadFile(const char* fileName, std::stringstream& result) {
	try {

		std::ifstream file;
		file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		file.open(fileName);
		result << file.rdbuf();
		file.close();
		return true;
	}
	catch (std::exception e) {
		std::cerr << "Error" << e.what() << ":" << fileName << std::endl;
		return false;
	}
}
void Shader::createShader(GLenum shaderType, std::string shaderSource, GLuint& shaderProgram)
{
	const char* buff = (shaderSource).c_str();
	//creamos el programa de shaders (programa final de la union de los shaders)


	//creamos el shader
	GLuint shader = glCreateShader(shaderType);

	//asociamos el source codigo al objeto del shader
	glShaderSource(shader/*objeto del shader  */, 1/*queremos crear un shader*/, &buff/*codigo del shader*/, NULL);

	//compilamos el shader
	glCompileShader(shader);
	//checkeamos la compilacion
	checkShaderCompilation(shader);

	//asociamos el shader al programa de shaders
	glAttachShader(shaderProgram, shader);
	//destruimos el identificador que ya no nos es util

	glDeleteShader(shader);
}
void Shader::bind()
{
	glUseProgram(m_shaderProgram);
}
void Shader::unBind()
{
	glUseProgram(0);
}
GLint Shader::checkShaderCompilation(GLuint shader)
{
	GLint succes = 0;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);

	if (!succes) {
		glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cerr << infoLog << std::endl;
	}
	return succes;
}
GLint Shader::checkProgramLink(GLuint program)
{
	GLint succes = 0;
	GLchar infoLog[512];
	glGetShaderiv(program, GL_LINK_STATUS, &succes);

	if (!succes) {
		glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
		std::cerr << infoLog << std::endl;
	}
	return succes;
}
Shader::~Shader()
{
	glDeleteProgram(m_shaderProgram);
}
void Shader::uniform1i(const char* name, int value)
{
	glUniform1i(getUniformLocation(name), value);
}

void Shader::uniform4f(const char* name, GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glUniform4f(getUniformLocation(name), r, g, b, a);
}
void Shader::uniform1f(const char* name, GLfloat r)
{
	bind();
	glUniform1f(getUniformLocation(name), r);
	unBind();
}
void Shader::uniform3f(const char* name, GLfloat r, GLfloat g, GLfloat b)
{
	bind();
	glUniform3f(getUniformLocation(name), r, g, b);
	unBind();
}
void Shader::uniform2f(const char* name, GLfloat r, GLfloat g)
{
	bind();
	glUniform2f(getUniformLocation(name), r, g);
	unBind();
}
void Shader::uniformMatrix4f(const char* name, GLsizei count, GLboolean transpose, const GLfloat* matrix)
{
	bind();
	glUniformMatrix4fv(getUniformLocation(name), count, transpose, matrix);
	unBind();
}

GLuint Shader::getUniformLocation(const char* name)
{


	//obtenemos la localizacion de la variable del shader a la que le queremos asignar datos
	GLuint location = glGetUniformLocation(m_shaderProgram, name);


	//retornamos esa localizacion
	return location;
}

