#include "Camera.hpp"

Camera::Camera(bool *keysVector, GLFWwindow *window)
	: _upVector(glm::vec3(0.0f, 1.0f, 0.0f)), projection(_projection), view(_view), view2D(_view2D), projection2D(_projection2D){
	glfwGetFramebufferSize(window, &_width, &_height);
	_view = glm::mat4();
	_projection = glm::mat4();
	_view2D = glm::translate(_view2D, glm::vec3(0.0f, 0.0f, -1.0f));
	_projection2D = glm::ortho(0.0f, (float)_width, 0.0f, (float)_height, 0.0f, 1000.0f);
	_lastTime = (float)glfwGetTime();
	_keys = keysVector;
	_speedValue = 10.0f;
	_cameraPosition = new glm::vec3(0.0f, 0.0f, 0.0f);
	_cameraFront = new glm::vec3(0.0f, 0.0f, -1.0f);
	_cameraUp= new glm::vec3(_upVector);
	_cameraRight = new glm::vec3(glm::cross(*_cameraFront, *_cameraUp));
	_cameraSpeed = new glm::vec3(0.0f, 0.0f, 0.0f);
	_hasGravity = false;
	_gravityValue = 150.0f;
	_gravity = new glm::vec3(0.0f, -1.0f, 0.0f);
}
Camera::Camera(glm::vec3 position, bool *keysVector, GLFWwindow *window) : Camera(keysVector, window){
	*_cameraPosition = position;
}
Camera::Camera(float x, float y, float z, bool *keysVector, GLFWwindow *window) : Camera(keysVector, window) {
	_cameraPosition->x = x;
	_cameraPosition->y = y;
	_cameraPosition->z = z;
}
void Camera::enableGravity() {
	_hasGravity = true;
}
void Camera::disableGravity() {
	_hasGravity = false;
}
void Camera::jump(float value) {
	if(_hasGravity && (_cameraPosition->y == 0))
		_cameraSpeed->y = value;
}
void Camera::setSpeedValue(float value) {
	_speedValue = value;
}
void Camera::Update() {
	int somethingPressed = 0;
	glm::vec3 speedDirection = glm::vec3(0, 0, 0);
	if (_keys[GLFW_KEY_W]) {
		somethingPressed++;
		speedDirection += *_cameraFront;
	}
	if (_keys[GLFW_KEY_S]) {
		somethingPressed++;
		speedDirection -= *_cameraFront;
	}
	if (_keys[GLFW_KEY_A]) {
		somethingPressed++;
		speedDirection -= *_cameraRight;
	}
	if (_keys[GLFW_KEY_D]) {
		somethingPressed++;
		speedDirection += *_cameraRight;
	}
	if ((somethingPressed > 1) && (glm::length(speedDirection) > 1.0f) )
		speedDirection = glm::normalize(speedDirection);

	_cameraSpeed->x = _cameraSpeed->z = 0;
	*_cameraSpeed += _speedValue * speedDirection;

	float currentTime = (float)glfwGetTime();

	if (_hasGravity)
		(*_cameraSpeed) += ((*_gravity) * _gravityValue * (currentTime - _lastTime));

	*_cameraPosition += ((*_cameraSpeed) * (currentTime - _lastTime));

	if (_cameraPosition->y <= 0.0f) {
		_cameraPosition->y = 0.0f;
		_cameraSpeed->y = 0;
	}
	_lastTime = currentTime;

	_view = glm::lookAt(*_cameraPosition, (*_cameraPosition) + (*_cameraFront), *_cameraUp);
	_projection = glm::perspective(45.0f, (float)_width / (float)_height, 0.1f, 1000.0f);
}
Camera::~Camera() {
	delete(_cameraPosition);
	delete(_cameraFront);
	delete(_cameraUp);
	delete(_cameraRight);
	delete(_cameraSpeed);
	delete(_gravity);
}