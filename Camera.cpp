#include "Camera.hpp"

Camera::Camera(bool *keysVector)
	: _upVector(glm::vec3(0.0f, 1.0f, 0.0f)), fov(_fov), view(_view){
	_fov = 45.0f;
	_view = glm::mat4();
	_lastTime = (float)glfwGetTime();
	_keys = keysVector;
	_speedValue = 10.0f;
	_cameraPosition = new glm::vec3(0.0f, 0.0f, 0.0f);
	_cameraFront = new glm::vec3(0.0f, 0.0f, -1.0f);
	_cameraUp= new glm::vec3(_upVector);
	_cameraRight = new glm::vec3(glm::cross(*_cameraFront, *_cameraUp));
	_cameraSpeed = new glm::vec3(0.0f, 0.0f, 0.0f);
}
Camera::Camera(glm::vec3 position, bool *keysVector) : Camera(keysVector){
	*_cameraPosition = position;
}
Camera::Camera(float x, float y, float z, bool *keysVector) : Camera(keysVector) {
	_cameraPosition->x = x;
	_cameraPosition->y = y;
	_cameraPosition->z = z;
}
void Camera::setSpeedValue(float value) {
	_speedValue = value;
}
void Camera::Update() {
	int somethingPressed = 0;
	//std::cout << "Current position: (" << _cameraPosition->x << ", " << _cameraPosition->y << ", " << _cameraPosition->z << ")" << std::endl;
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
	if ((somethingPressed > 2) && (glm::length(speedDirection) > 1.0f) )
		speedDirection = glm::normalize(speedDirection);

	*_cameraSpeed = _speedValue * speedDirection;
	//if (somethingPressed)
		//std::cout << "cameraSpeed: (" << _cameraSpeed->x << ", " << _cameraSpeed->y << ", " << _cameraSpeed->z << ")" << std::endl;

	float currentTime = (float)glfwGetTime();
	*_cameraPosition += ((*_cameraSpeed) * (currentTime - _lastTime));
	_lastTime = currentTime;

	_view = glm::lookAt(*_cameraPosition, (*_cameraPosition) + (*_cameraFront), *_cameraUp);
}
Camera::~Camera() {
	delete(_cameraPosition);
	delete(_cameraFront);
	delete(_cameraUp);
	delete(_cameraRight);
	delete(_cameraSpeed);
}