#include "Camera.hpp"

Camera::Camera(bool *keysVector, GLFWwindow *window)
	// sets the values of the const variables
	: _upVector(glm::vec3(0.0f, 1.0f, 0.0f)), projection(_projection), view(_view), view2D(_view2D), projection2D(_projection2D){
	// Gets variables ready
	glfwGetFramebufferSize(window, &_width, &_height);
	_view = glm::mat4();
	_projection = glm::mat4();
	_view2D = glm::translate(_view2D, glm::vec3(0.0f, 0.0f, -1.0f));
	_projection2D = glm::ortho(0.0f, (float)_width, 0.0f, (float)_height, 0.0f, 1000.0f);
	_projection = glm::perspective(45.0f, (float)_width / (float)_height, 0.1f, 1000.0f);
	_lastTime = (float)glfwGetTime();
	_keys = keysVector;
	_speedValue = 30.0f;
	_cameraPosition = new glm::vec3(0.0f, 0.0f, 0.0f);
	_cameraFront = new glm::vec3(0.0f, 0.0f, -1.0f);
	_cameraUp= new glm::vec3(_upVector);
	_cameraRight = new glm::vec3(glm::cross(*_cameraFront, *_cameraUp));
	_cameraSpeed = new glm::vec3(0.0f, 0.0f, 0.0f);
	_hasGravity = false;
	_gravityValue = 150.0f;
	_gravity = new glm::vec3(-1.0f *_upVector);
}

Camera::Camera(glm::vec3 position, bool *keysVector, GLFWwindow *window) : Camera(keysVector, window){
	// Does the basic constructor and sets camera position
	*_cameraPosition = position;
}

Camera::Camera(float x, float y, float z, bool *keysVector, GLFWwindow *window) : Camera(keysVector, window) {
	// Same as above, different method
	_cameraPosition->x = x;
	_cameraPosition->y = y;
	_cameraPosition->z = z;
}
// Some setters
void Camera::enableGravity() {
	_hasGravity = true;
}

void Camera::disableGravity() {
	_hasGravity = false;
}

void Camera::jump(float value) {
	// Only jumps if gravity is enabled and position is above ground
	if(_hasGravity && (_cameraPosition->y == 0))
		_cameraSpeed->y = value;
}

void Camera::setSpeedValue(float value) {
	_speedValue = value;
}

void Camera::Update() {
	// sometingPressed stores how many movement keys are currently pressed
	int somethingPressed = 0;
	glm::vec3 speedDirection = glm::vec3(0, 0, 0);
	// Checks if the 4 movement keys are pressed
	if (_keys[GLFW_KEY_W]) {
		// Increments the counter
		somethingPressed++;
		// Sums the direction selected with speedDirection
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
	// Normalizes the vector if necessary
	if ((somethingPressed > 1) && (glm::length(speedDirection) > 1.0f) )
		speedDirection = glm::normalize(speedDirection);
	// Resets camera speed on the xz plane
	_cameraSpeed->x = _cameraSpeed->z = 0;
	// Applies the new desired speed
	*_cameraSpeed += _speedValue * speedDirection;
	// gets current time to calculate deltaTime between updates
	float currentTime = (float)glfwGetTime();

	// Applies gravity to y axis speed if necessary
	if (_hasGravity)
		(*_cameraSpeed) += ((*_gravity) * _gravityValue * (currentTime - _lastTime));

	// Updates camera position
	*_cameraPosition += ((*_cameraSpeed) * (currentTime - _lastTime));
	// If the camera goes below ground(y = 0), puts it back above ground and stops y axis movement
	if (_cameraPosition->y <= 0.0f) {
		_cameraPosition->y = 0.0f;
		_cameraSpeed->y = 0;
	}
	// Updates the time
	_lastTime = currentTime;
	// Updates view matrix based on new camera position
	_view = glm::lookAt(*_cameraPosition, (*_cameraPosition) + (*_cameraFront), *_cameraUp);
}
Camera::~Camera() {
	// Frees allocated memory
	delete(_cameraPosition);
	delete(_cameraFront);
	delete(_cameraUp);
	delete(_cameraRight);
	delete(_cameraSpeed);
	delete(_gravity);
}