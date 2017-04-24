#include "Polygon.hpp"

void Polygon::generateVertices() {
	// Create vertices and index arrays
	_vertices = new GLfloat[_sides * 3];
	_indices = new GLuint[(_sides - 2) * 3];
	// Determines angle to spin and create new vertices
	float tetha = 2 * PI / (float)_sides;
	// Fills the arrays
	for (int i = 0; i < _sides; i++) {
		// The polygon is created with a radius of 1 by default
		_vertices[3 * i] = (1.0f * sin(tetha * i));
		_vertices[(3 * i) + 1] = (1.0f * cos(tetha * i));
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
	_x  = x;
	_y  = y;
	_radius = radius;
	_sides = sides;
	glfwGetFramebufferSize(window, &_width, &_height);
	// Gets arrays ready
	generateVertices();
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
	_speedDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	_speedValue = 0;
	_startTime = glfwGetTime();
}

float Polygon::x() {
	return _x;
}

float Polygon::y() {
	return _y;
}

void Polygon::setSpeed(glm::vec3 direction, float value) {
	_speedDirection = glm::normalize(direction);
	_speedValue = value;
}

void Polygon::setShaderProgram(GLuint *shaderProgram) {
	_shaderProgram = shaderProgram;
}

void Polygon::Update() {

	// Applies the speed to x and y, using the window coordinates

	glm::vec4 aux = glm::vec4(_x, _y, 0.0f, 1.0f);
	glm::mat4 transform;
	// Gets the current time and applies transform based on deltaTime between updates
	float currentTime = (GLfloat)glfwGetTime();
	transform = glm::translate(transform, (_speedDirection * _speedValue) * (currentTime - _startTime));
	// Saves the time of the current update
	_startTime = currentTime;
	// Applies the translation and updates current position
	aux = transform * aux;
	_x = aux.x;
	_y = aux.y;
	// Check if the object is out of bounds
	if (_x + _radius < 0 || _x - _radius > _width || _y + _radius < 0 || _y - _radius > _height) {
		// If the object is out of bounds, determines the location where it must respawn
		do {
			// Reverses speed and looks for the first position out of bounds
			_x -= _speedDirection.x;
			_y -= _speedDirection.y;
		} while (_x + _radius >= 0 && _x - _radius <= _width && _y + _radius >= 0 && _y - _radius <= _height);
		// Moves a little bit to ensure it is back inside the boundaries
		_x += _speedDirection.x;
		_y += _speedDirection.y;
	}
}

void Polygon::Draw() {
	// Sets shader program to be used
	glUseProgram(*_shaderProgram);

	// model transform changes from local space to world space, view changes from world to view space
	// and projection changes from view space to clip space. glTransform contains the resulting transform matrix
	glm::mat4 model, view, projection, gltransform;
	glm::vec3 glSpeed = _speedDirection * _speedValue;
	// the translation moves the polygon to it's location in world space
	model = glm::translate(model, glm::vec3(_x, _y, 0.0f));
	// the scale applies the polygon radius
	model = glm::scale(model, glm::vec3(_radius, _radius, 1.0f));
	// moves the camera back a bit
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));
	// applies orthogonal view
	projection = glm::ortho(0.0f, (float)_width, 0.0f, (float)_height, 0.0f, 1000.0f);
	// Passes the resulting transform matrix to the vertex shader
	gltransform = projection * view * model;

	// Applies transform to shader program
	GLuint transformLoc = glGetUniformLocation(*_shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(gltransform));

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