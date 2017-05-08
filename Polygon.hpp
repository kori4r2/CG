#pragma once
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.hpp"

#define PI 3.14159265359

class Polygon {
private:
	// _x and _y save the initial object position (using custom window coordinates)
	// _radius is the polygon radius (also using the custom coordinates)
	// _startTime is the time when the movement started, everytime setSpeed is called or the object
	// goes out of the screen, it should be reset with _startTime = glfwGetTime() to make object come back
	// to the starting position
	Camera *_camera;
	float _x, _y, _radius, _startTime, _speedValue;
	int _sides, _width, _height;
	GLfloat *_vertices;
	GLuint *_indices;
	GLuint _VAO, _VBO, _EBO;
	glm::vec3 _speedDirection;
	GLuint *_shaderProgram;

	// Generates vertices and indices based on the polygon characteristics
	void generateVertices();

public:
	//	Creates a new polygon object. These x and y variables consider the bottom left corner of the
	// window as the point of origin and window width and height as maximum values
	Polygon(float x, float y, float radius, int sides, Camera *camera, GLFWwindow *window);
	// Some getters and setters
	float x();
	float y();
	//	The direction vector will be normalized inside the function
	void setSpeed(glm::vec3 direction, float value);
	void setShaderProgram(GLuint *shaderProgram);
	// Update and draw
	void Update();
	void Draw();
	~Polygon();
};
