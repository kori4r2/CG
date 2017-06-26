#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW\glfw3.h>

class Camera {
private:
	// Reference to the variables containing mouse position and variable to store the position from last update
	double *_mouseX, *_mouseY, _lastMouseX, _lastMouseY, *_yScroll;
	// Yaw and pitch value are used to store the current rotation value of the camera
	GLfloat _yaw, _pitch, _fov;
	// _speedValue stores the speed for camera movement on xz plane, _lastTime is use for updates, _gravityValue stores
	// absolute value of gravity force, _eyeHeight stores the value for the camera elevation related to it's position
	float _speedValue, _lastTime, _gravityValue, _eyeHeight;
	// _width and _height are window size parameters
	int _width, _height;
	// _keys is a reference for the key state vector, _hasGravity determines wether the object should be affected by gravity
	// _firstMouse indicates if this is the first mouse movement
	bool *_keys, _hasGravity, _firstMouse, _crouching;
	// _upVector is a reference to up position in world space coordinates
	const glm::vec3 _upVector;
	// _cameraPosition stores the current camera position in world space coordinates, _cameraUp, _cameraFront and _cameraRight
	// are normalized vectors pointing to those directions relative to the camera, _gravity contains the directions of the
	// gravity force
	glm::vec3 *_cameraPosition, *_cameraUp, *_cameraFront, *_cameraRight, *_cameraSpeed, *_gravity;
	// The for transformation matrices to be determined according to camera position and window parameters
	glm::mat4 _view, _projection, _view2D, _projection2D;

public:
	// These four values are references to the transformation matrices, allowing them to be accessed from outside the class,
	// but not altered
	const glm::mat4 &view, &projection, &view2D, &projection2D;
	// This is a reference value to the camera front vector, needed for light calculations(NOT REALLY)
	glm::vec3* const& cameraFront;
	// The basic constructor only needs a reference to the keys vector and the window
	Camera(bool *keysVector, GLFWwindow *window, double *mousex, double *mousey, double *yScroll);
	// The other constructors can set the camera position as well
	Camera(glm::vec3 position, bool *keysVector, GLFWwindow *window, double *mousex, double *mousey, double *yScroll);
	Camera(float x, float y, float z, bool *keysVector, GLFWwindow *window, double *mousex, double *mousey, double *yScroll);
	// Some setters
	void enableGravity();
	void disableGravity();
	// Getter of camera position taking eye level into account
	glm::vec3 viewPosition();
	// A getter for camera position, taking the control key press into consideration
	// The jump function is basically a setter for the y axis speed of the camera
	void jump(float value);
	void setSpeedValue(float value);
	// Update only, no draw
	void Update();
	~Camera();
};