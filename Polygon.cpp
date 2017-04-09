#include "Polygon.hpp"

void Polygon::generateVertices(GLFWwindow *window) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	// Gets the normalized values of x and y based on the the -1 to 1 coordinate system used by opengl
	float normalX = (2 * _x / width) - 1;
	float normalY = (2 * _y / height) - 1;
	// Create vertices and index arrays
	_vertices = new GLfloat[_sides * 3];
	_indices = new GLuint[(_sides - 2) * 3];
	// Determines angle to spin and create new vertices
	float tetha = 2 * PI / (float)_sides;
	// Fills the arrays
	for (int i = 0; i < _sides; i++) {
		// The values on the x and y axis have different scaling to avoid problems with different resolutions
		_vertices[3 * i] = normalX + ((2 * _radius / width) * sin(tetha * i));
		_vertices[(3 * i) + 1] = normalY + ((2 * _radius / height) * cos(tetha * i));
		_vertices[(3 * i) + 2] = 1.0f;
	}
	// Saves vertices in order to draw all the triangles that make the full polygon
	for (int i = 0; i < _sides - 2; i++) {
		_indices[3 * i] = 0;
		_indices[(3 * i) + 1] = i + 1;
		_indices[(3 * i) + 2] = i + 2;
	}
}

Polygon::Polygon(float x, float y, float radius, int sides, GLFWwindow *window) {
	// Gets variables ready
	_x = x;
	_y = y;
	_radius = radius;
	_sides = sides;
	// Gets arrays ready
	generateVertices(window);
	// Set EBO, VAO and VBO for drawing
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

	// Sets movement related variables
	_speed = glm::vec3(0.0f, 0.0f, 0.0f);
	_startTime = glfwGetTime();
}

float Polygon::x() {
	return _x;
}

float Polygon::y() {
	return _y;
}

void Polygon::setSpeed(glm::vec3 direction, float value) {
	_speed = direction * value;
	_startTime = glfwGetTime();
}

void Polygon::setShaderProgram(GLuint *shaderProgram) {
	_shaderProgram = shaderProgram;
}

void Polygon::Update() {
	// Sets shader program to be used
	glUseProgram(*_shaderProgram);

	// Changes transform acording to current speed of the object
	glm::mat4 transform;
	transform = glm::translate(transform, _speed * ((GLfloat)glfwGetTime() - _startTime));
	// To do: apply angular speed
	// Applies transform to shader program
	GLuint transformLoc = glGetUniformLocation(*_shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// To do: check if the object is out of bounds
	// (_x + _radius < 0 || _x - _radius > width || _y + _radius < 0 || _y - _radius > height) shoud work, but
	// _x and _y values are not being altered

	// To do:
	// if(object out of bounds)
	//		_startTime = glfwGetTime();
}

void Polygon::Draw() {
	// Draws the object, no surprises
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, (_sides - 2) * 3, GL_UNSIGNED_INT, _indices);
	glBindVertexArray(0);
}

Polygon::~Polygon() {
	// Frees allocated memory
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_EBO);
	delete(_vertices);
	delete(_indices);
}