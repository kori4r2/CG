#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW\glfw3.h>

class Camera {
private:
	float _speedValue, _lastTime, _fov;
	bool *_keys;
	const glm::vec3 _upVector;
	glm::vec3 *_cameraPosition, *_cameraUp, *_cameraFront, *_cameraRight, *_cameraSpeed;
	glm::mat4 _view;
public:
	const float &fov;
	const glm::mat4 &view;
	Camera(bool *keysVector);
	Camera(glm::vec3 position, bool *keysVector);
	Camera(float x, float y, float z, bool *keysVector);
	void setSpeedValue(float value);
	void Update();

	~Camera();
};