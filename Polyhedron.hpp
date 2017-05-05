#pragma once
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.hpp"

class Polyhedron {
private:
	virtual void generateVertices() = 0;

protected:
	static const GLfloat cubeVertices[], cubeIndices[], tetrahedronVertices[], tetrahedronIndices[];
	float _x, _y, _z, _startTime, _currentTime, _radius, _speedValue, _angularSpeedValue, _angle;
	int _width, _height, _nFaces, _nSidesFaces;
	GLfloat *_vertices;
	GLuint *_indices;
	glm::vec3 *_speedDirection, *_rotationAxis;
	GLuint _VAO, _VBO, _EBO;
	GLuint *_shaderProgram;
	Camera *_camera;

public:
	Polyhedron(float x, float y, float z, float radius, GLFWwindow *window, Camera *camera);
	// Some getters and setters
	float x();
	float y();
	float z();
	//	The direction vector will be normalized inside the function
	void setSpeed(glm::vec3 direction, float value);
	void setSpeed(float value);
	void setAngle(float value);
	void setAngularSpeed(glm::vec3 axis, float value);
	void setAngularSpeed(float value);
	void setShaderProgram(GLuint *shaderProgram);
	void Update();
	void Draw();
	~Polyhedron();
};