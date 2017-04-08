#pragma once
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.14159265359

class Polygon {
private:
	float _x, _y, _radius;
	int _sides;
	GLfloat *_vertices;
	GLuint *_indices;
	GLuint _VAO, _VBO, _EBO;
	glm::vec3 _speed;

	void generateVertices(GLFWwindow*);

public:
	Polygon(float x, float y, float radius, int sides, GLFWwindow *window);
	void setSpeed(glm::vec3 direction, float value);
	void Update();
	void Draw();
	~Polygon();
};
