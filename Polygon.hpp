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
	// _x and _y save the object position (using screen space coordinates)
	// _radius is the polygon radius (also using screen space coordinates)
	// _startTime is the time of the last update and _speedValue is the absolute value of the object speed
	float _x, _y, _radius, _startTime, _speedValue;
	// _sides is how many sides the polygon has, _width and _height are window size parameters
	int _sides, _width, _height;
	// _speedDirection is a normalized vector of the object speed
	glm::vec3 _speedDirection;
	// Variables needed for drawing
	GLfloat *_vertices;
	GLuint *_indices;
	GLuint _VAO, _VBO, _EBO;
	// Reference to the shader program
	GLuint *_shaderProgram;
	// A reference to the camera object
	Camera *_camera;

	// Generates vertices and indices based on the polygon characteristics
	void generateVertices();

public:
	//	Creates a new polygon object. The x, y and radius variables consider a screen space coordinate system
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
