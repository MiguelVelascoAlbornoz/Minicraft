#include "Camera.h"

const vec2 defaultSize = vec2(1000, 1000);
Camera::Camera(vec3 initialPos, float initialZoom,const vec2& windowSize, float initialFov) : pos(initialPos), zoom(initialZoom),windowSize(windowSize),fov(initialFov){
	
	scaleMatrix = mat4(1.0f);
	rotationMatrix = mat4(1.0f);
	perspectiveMatrix = mat4(1.0f);
	modelMatrix = mat4(1.0f);
	direction = vec3(0, 0, 0);
	shader = new  Shader();
	updateProjection(fov, nearPlane, farPlane);
	setZoom(zoom);
	setDirection(direction);
	setPos(initialPos);
}

void Camera::unBindShader()
{
	shader->unBind();
}
void Camera::bindShader()
{
	shader->bind();
}
Camera::~Camera()
{
	delete shader;
}
void Camera::setZoom(float zoom) {
	this->zoom = zoom;
	scaleMatrix = scale(mat4(1.0f), vec3(zoom, zoom, zoom));
	updateModelMatrix();
	

}
void Camera::setPos(vec3& newPos) {
	pos = newPos;
	updateModelMatrix();
	shader->uniform3f("playerPos",pos.x,pos.y,pos.z);
}
void Camera::setDirection(vec3& newDirection) {
	direction = newDirection;
	if (direction.x > 360) {
		direction.x = direction.x - 360 * floor(direction.x/360);
	} 
	if (direction.y > 360) {
		direction.y = direction.y - 360 * floor(direction.y / 360);
	}
	if (direction.z > 360) {
		direction.z = direction.z - 360 * floor(direction.z / 360);
	}

	rotationMatrix = rotate(mat4(1.0f), radians(direction.x), vec3(-1, 0, 0));
	rotationMatrix = rotate(rotationMatrix, radians(direction.y), vec3(0, -1, 0));
	rotationMatrix = rotate(rotationMatrix, radians(direction.z), vec3(0, 0, -1));
	updateModelMatrix();
}
void Camera::updateModelMatrix() {
	modelMatrix = perspectiveMatrix * translate(rotationMatrix, -pos) * scaleMatrix ;
	shader->uniformMatrix4f("model", 1, GL_FALSE, value_ptr(modelMatrix));
}
void Camera::updateProjection(float fov, float near, float far)
{

    perspectiveMatrix = glm::perspective(glm::radians(fov), windowSize.x/windowSize.y, near, far);

	updateModelMatrix();
}
