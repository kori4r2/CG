#include "Camera.hpp"

Camera::Camera(bool *keysVector, GLFWwindow *window, double *mousex, double *mousey, double *yScroll)
	// sets the values of the const variables
	: _upVector(glm::vec3(0.0f, 1.0f, 0.0f)), projection(_projection), view(_view),
	view2D(_view2D), projection2D(_projection2D), cameraFront(_cameraFront){
	// Gets variables ready
	_eyeHeight = 20.0f;
	_crouching = false;
	_firstMouse = true;
	_mouseX = mousex;
	_mouseY = mousey;
	_lastMouseX = *mousex;
	_lastMouseY = *mousey;
	_yaw = 0.0f;
	_pitch = 0.0f;
	_fov = 45.0f;
	_yScroll = yScroll;

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

Camera::Camera(glm::vec3 position, bool *keysVector, GLFWwindow *window, double *mousex, double *mousey, double *yScroll)
	: Camera(keysVector, window, mousex, mousey, yScroll){
	// Does the basic constructor and sets camera position
	*_cameraPosition = position;
}

Camera::Camera(float x, float y, float z, bool *keysVector, GLFWwindow *window, double *mousex, double *mousey, double *yScroll)
	: Camera(keysVector, window, mousex, mousey, yScroll) {
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

glm::vec3 Camera::viewPosition() {
	if (_crouching)
		return *_cameraPosition;
	else
		return ((*_cameraPosition) - glm::vec3(0.0f, _eyeHeight, 0.0f));
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
	// Obs.: For some unknown reason updating _cameraRight vector with cross product breaks stuff
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

	// If any shift key is being pressed, increases speed
	_speedValue *= (_keys[GLFW_KEY_LEFT_SHIFT] || _keys[GLFW_KEY_RIGHT_SHIFT]) ? 2.5f : 1.0f;

	// Applies the new desired speed
	*_cameraSpeed += _speedValue * speedDirection;

	// reverts _speedValue back to it's original value
	_speedValue /= (_keys[GLFW_KEY_LEFT_SHIFT] || _keys[GLFW_KEY_RIGHT_SHIFT]) ? 2.5f : 1.0f;

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

	// Applies _eyeHeight elevation if a CONTROL key is not pressed
	_crouching = (_keys[GLFW_KEY_LEFT_CONTROL] || _keys[GLFW_KEY_RIGHT_CONTROL]);
	if(!_crouching)
		_cameraPosition->y += _eyeHeight;

	// Updates FOV based on yScroll from the mouse
	_fov -= *_yScroll;
	if (_fov <= 1.0f)
		_fov = 1.0f;
	if(_fov >= 45.0f)
		_fov = 45.0f;
	*_yScroll = 0.0f;

	// Updates view matrix based on new camera position
	_view = glm::lookAt(*_cameraPosition, (*_cameraPosition) + (*_cameraFront), *_cameraUp);
	_projection = glm::perspective(glm::radians(_fov), (float)_width / (float)_height, 0.1f, 1000.0f);

	// Undoes the _eyeHeight elevation if needed
	if (!_crouching)
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