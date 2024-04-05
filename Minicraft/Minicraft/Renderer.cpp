#include "Renderer.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) glClearError();\
        x;\
        ASSERT(glLogCall(#x, __FILE__, __LINE__) )
Renderer::~Renderer()
{

	glfwDestroyWindow(window);
	glfwTerminate();
}
void Renderer::toggleFullscreen() {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	vec2 windowPos = vec2(0, 0);
	if (isFullscreen == false) {
		windowSize.x = (float)(mode->width);
		windowSize.y = (float)(mode->height);
		isFullscreen = true;
	} else {
		windowSize.x = (float)mode->width / 2;
		windowSize.y = (float)mode->height / 2;
		
	    windowPos = vec2(mode->width/6, mode->height/6);
		monitor = NULL;
		isFullscreen = false;
	}
	glfwSetWindowMonitor(window, monitor, windowPos.x, windowPos.y, (int)windowSize.x, (int)windowSize.y, mode->refreshRate);
	glViewport(0, 0, (int)windowSize.x, (int)windowSize.y);
	glfwSwapInterval(1);//Vysinc
}
bool Renderer::initRenderer(bool fullscreen)
{
	isFullscreen = fullscreen;

	//Initialize GLFW
	if (!glfwInit()) {
		std::cout << "glfw init error" << std::endl; 
		return false;
	}
	//glfw Config
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_REFRESH_RATE, 120);
	glfwSetErrorCallback(Renderer::errorCallback);

	//Create window
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if (isFullscreen) {
		windowSize.x = (float)(mode->width);
		windowSize.y = (float)(mode->height);
	} else {
		windowSize.x = (float)mode->width / 2;
		windowSize.y = (float)mode->height / 2;
		monitor = NULL;
	}
	window = glfwCreateWindow((int)windowSize.x, (int)windowSize.y, "OpenGLBase", monitor, NULL);

	if (!window) {
		std::cout << "Window INIT error" << std::endl;
		return false;
	}
	//Window Config
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);//Vysinc

	//Initialize glew
	GLenum glewInitErr = glewInit();
	if (glewInitErr != GLEW_OK) {
		std::cerr << "glew init has failed: " << glewGetErrorString(glewInitErr) << std::endl;
		return false;
	}
	//glew config
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST); 
	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//Mostrar datos el ambiente de trabajo 
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Renderizador: " << renderer << std::endl;
	std::cout << "Version de openGL soportada: " << version << std::endl;


	

	return true;
}

void Renderer::errorCallback(int error, const char* description)
{
	std::cout << "Error: " << error << ": " << description << std::endl;
}
bool Renderer::glLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGl error] (" << error << "):" << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::fill(Shader* shader, vec3* color)
{
	this->color = *color;
	shader->uniform3f("color",color->x, color->y, color->z);
}
void Renderer::background( GLfloat r, GLfloat g, GLfloat b) {
	glClearColor(r, g,b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Renderer::update() {
	glfwSwapBuffers(window);
}

void Renderer::fill(Shader* shader,GLfloat r, GLfloat g, GLfloat b)
{
	color.x = r;
	color.y = g;
	color.z = b;
	shader->uniform3f("color", color.x, color.y, color.z);
}
void Renderer::drawRect(float px, float py, float width, float height, float z) {
	// Define los vértices del rectángulo
	float vertices[] = {
		px, py + height, z,
		px + width, py + height,z,
		px, py, z,
		px + width, py + height, z,
		px + width, py, z,
		px, py,z
	};

	// Genera y configura el VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configura los atributos de vértices
	GLuint attribLocation = 0;
	GLint size = 3;
	GLenum type = GL_FLOAT;
	GLboolean normalized = GL_FALSE;
	GLsizei stride = 3 * sizeof(float);
	const void* offset = 0;

	glVertexAttribPointer(attribLocation, size, type, normalized, stride, offset);
	glEnableVertexAttribArray(attribLocation);

	// Dibuja el rectángulo
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Limpia y desactiva el VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);
}

