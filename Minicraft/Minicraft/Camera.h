#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Shader.h"
using namespace glm;
class Camera
{
public:
	Camera(vec3 initialPos, float initialZoom, const vec2& windowSize, float fov);

	~Camera();

	vec3 getPos() { return pos; }
	void setZoom(float zoom);
	float getZoom() { return zoom; };
	Shader* shader;
	void bindShader();
	void setPos(vec3& newPos);
	void setDirection(vec3& newDirection);

	void updateModelMatrix();
	void updateProjection(float fov, float near, float far);
	void unBindShader();
	float getFov() { return fov; };
	float getNear() { return nearPlane; };
	float getFar() { return farPlane; };
	float speed = 2.0f;
	vec3 getDirection() { return direction; };
	float sensibility = 10;
	private :
	vec3 direction;

	float fov;
	mat4 scaleMatrix;
	mat4 perspectiveMatrix;
	mat4 rotationMatrix;
	mat4 modelMatrix = mat4(1.0f);
	float nearPlane = 0.1;
	float farPlane = 100.0f;
	static const vec2& defaultSize;
	const vec2& windowSize;
	vec3 pos;
	float zoom;

};

