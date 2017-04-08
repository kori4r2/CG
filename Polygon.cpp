#include "Polygon.hpp"
Polygon::Polygon(float x, float y, float radius, int sides, GLFWwindow *window) {
	_x = x;
	_y = y;
	_radius = radius;
	_sides = sides;
	generateVertices(window);
	glGenBuffers(1, &_EBO);
	glGenBuffers(1, &_VBO);
	glGenVertexArrays(1, &_VAO);

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * _sides * sizeof(GLfloat), _vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _sides * sizeof(GLuint), _indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Polygon::setSpeed(glm::vec3 direction, float value) {
	_speed = direction * value;
}

void Polygon::Update() {
}

void Polygon::Draw() {
	// To do (no internet)
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, (_sides - 2) * 3, GL_UNSIGNED_INT, _indices);
	glBindVertexArray(0);
}

void Polygon::generateVertices(GLFWwindow *window) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float normalX = (2 * _x / width) - 1;
	float normalY = (2 * _y / height) - 1;
	// Create vertices and index arrays
	_vertices = new GLfloat[_sides * 3];
	_indices = new GLuint[(_sides - 2) * 3];
	// Determines angle to spin and create new vertices
	float tetha = 2*PI / (float)_sides;
	// Fills the arrays
	for (int i = 0; i < _sides; i++) {
		_vertices[3 * i] = normalX + ( (2 * _radius / width) * sin(tetha * i));
		_vertices[(3 * i) + 1] = normalY + ( (2 * _radius / height) * cos(tetha * i));
		_vertices[(3 * i) + 2] = 1.0f;
	}
	for (int i = 0; i < _sides - 2; i++) {
		_indices[3 * i] = 0;
		_indices[(3 * i) + 1] = i + 1;
		_indices[(3 * i) + 2] = i + 2;
	}
}

Polygon::~Polygon() {
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
	//glDeleteBuffers(1, &_EBO);
	delete(_vertices);
	delete(_indices);
}