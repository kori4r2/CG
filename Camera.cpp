#include "Camera.hpp"

Camera::Camera(bool *keysVector, GLFWwindow *window, double *mousex, double *mousey)
	// sets the values of the const variables
	: _upVector(glm::vec3(0.0f, 1.0f, 0.0f)), projection(_projection), view(_view), view2D(_view2D), projection2D(_projection2D){
	// Gets variables ready
	_eyeHeight = 5.0f;
	_firstMouse = true;
	_mouseX = mousex;
	_mouseY = mousey;
	_lastMouseX = *mousex;
	_lastMouseY = *mousey;
	_yaw = 0.0f;
	_pitch = 0.0f;

	glfwGetFramebufferSize(window, &_width, &_height);

	_view = glm::mat4();
	_projection = glm::mat4();
	_view2D = glm::translate(_view2D, glm::vec3(0.0f, 0.0f, -1.0f));
	_projection2D = glm::ortho(0.0f, (float)_width, 0.0f, (float)_height, 0.0f, 1000.0f);
	_projection = glm::perspective(45.0f, (float)_width / (float)_height, 0.1f, 1000.0f);

	_keys = keysVector;

	_lastTime = (float)glfwGetTime();
	_speedValue = 30.0f;
	_gravityValue = 200.0f;
	_hasGravity = false;

	_cameraPosition = new glm::vec3(0.0f, 0.0f, 0.0f);
	_cameraFront = new glm::vec3(0.0f, 0.0f, -1.0f);
	_cameraUp= new glm::vec3(_upVector);
	_cameraRight = new glm::vec3(glm::cross(*_cameraFront, *_cameraUp));
	_cameraSpeed = new glm::vec3(0.0f, 0.0f, 0.0f);
	_gravity = new glm::vec3(-1.0f *_upVector);
}

Camera::Camera(glm::vec3 position, bool *keysVector, GLFWwindow *window, double *mousex, double *mousey)
	: Camera(keysVector, window, mousex, mousey){
	// Does the basic constructor and sets camera position
	*_cameraPosition = position;
}

Camera::Camera(float x, float y, float z, bool *keysVector, GLFWwindow *window, double *mousex, double *mousey)
	: Camera(keysVector, window, mousex, mousey) {
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

	// If the mouse has not been moved yet
	if (_firstMouse) {
		// Checks if it's still in the initial position, if it moves, makes sure to not enter here again
		if((_lastMouseX != *_mouseX) || (_lastMouseY != *_mouseY))
			_firstMouse = false;
		// saves last position as current position to avoid weird camera jumps
		_lastMouseX = *_mouseX;
		_lastMouseY = *_mouseY;
	}

	// Checks how much the mouse moved and updates the last position
	double xOffset = _lastMouseX - *_mouseX;
	double yOffset = _lastMouseY - *_mouseY;
	_lastMouseX = *_mouseX;
	_lastMouseY = *_mouseY;

	// Gets the distance moved relative to the window size, multiplies by 90 degrees and then multiplies
	// by a "sensitivity" value
	xOffset = ( ((2 * xOffset) / _width) * glm::radians(90.0f)) * 0.1f;
	yOffset = ( ((2 * yOffset) / _height) * glm::radians(90.0f)) * 0.1f;

	// Makes sure the camera stays at an angle between 90.0 degrees and -90 degrees on the up/down rotation
	if (_pitch + yOffset >= glm::radians(90.0f)) {
		yOffset = glm::radians(89.9f) - _pitch;
		_pitch = glm::radians(89.9f);
	} else if (_pitch + yOffset <= glm::radians(-90.0f)) {
		yOffset = glm::radians(-89.9f) - _pitch;
		_pitch = glm::radians(-89.9f);
	}

	// Makes sure the yaw value stores the proper rotation value of the camera on the right/left rotation
	if ((_yaw > glm::radians(180.f)) || (_yaw < glm::radians(-180.0f)))
		_yaw -= (_yaw < 0) ? (glm::radians(-360.0f)) : glm::radians(360.0f);

	// Applies the offset rotation values
	_yaw += xOffset;
	_pitch += yOffset;
	
	// Uses the aux vector to aplly the rotations easily
	// The right/left rotation must be applied to both the _cameraFront and the _cameraRight vectors
	glm::vec4 aux = glm::vec4(*_cameraFront, 1.0f);
	*_cameraFront = glm::rotate(glm::mat4(), (float)xOffset, _upVector) * aux;
	aux = glm::vec4(*_cameraRight, 1.0f);
	*_cameraRight = glm::rotate(glm::mat4(), (float)xOffset, _upVector) * aux;
	// The up/down rotation must be applied to the _cameraFront vector, and the _cameraUp vector is updated using cross product
	aux = glm::vec4(*_cameraFront, 1.0f);
	*_cameraFront = glm::rotate(glm::mat4(), (float)yOffset, *_cameraRight) * aux;
	*_cameraUp = glm::cross(*_cameraRight, *_cameraFront);


	// sometingPressed stores how many movement keys are currently pressed
	int somethingPressed = 0;
	glm::vec3 speedDirection = glm::vec3(0, 0, 0);
	// Checks if the 4 movement keys are pressed
	if (_keys[GLFW_KEY_W]) {
		// Increments the counter
		somethingPressed++;
		// Sums the direction selected with speedDirection
		speedDirection += glm::vec3(glm::cross(_upVector, *_cameraRight));
	}
	if (_keys[GLFW_KEY_S]) {
		somethingPressed++;
		speedDirection -= glm::vec3(glm::cross(_upVector, *_cameraRight));
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
	if (_hasGravity && _cameraPosition->y > 0)
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
	_cameraPosition->y += _eyeHeight;
	_view = glm::lookAt(*_cameraPosition, (*_cameraPosition) + (*_cameraFront), *_cameraUp);
	_cameraPosition->y -= _eyeHeight;
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