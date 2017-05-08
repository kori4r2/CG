#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW\glfw3.h>

class Camera {
private:
	float _speedValue, _lastTime, _gravityValue;
	int _width, _height;
	bool *_keys, _hasGravity;
	const glm::vec3 _upVector;
	glm::vec3 *_cameraPosition, *_cameraUp, *_cameraFront, *_cameraRight, *_cameraSpeed, *_gravity;
	glm::mat4 _view, _projection, _view2D, _projection2D;

public:
	const glm::mat4 &view;
	const glm::mat4 &projection;
	const glm::mat4 &view2D;
	const glm::mat4 &projection2D;

	Camera(bool *keysVector, GLFWwindow *window);
	Camera(glm::vec3 position, bool *keysVector, GLFWwindow *window);
	Camera(float x, float y, float z, bool *keysVector, GLFWwindow *window);
	void enableGravity();
	void disableGravity();
	void jump(float value);
	void setSpeedValue(float value);
	void Update();

	~Camera();
};