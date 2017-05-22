#include "Plane.hpp"

Plane::Plane(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window) {
	// Gets variables ready
	_camera = camera;
	_x = x;
	_y = y;
	_z = z;
	_radius = radius;
	_gravityValue = 200.0f;
	_speed = new glm::vec3(0.0f, 0.0f, 0.0f);
	_rotationAxis = new glm::vec3(0.0f, 1.0f, 0.0f);
	_speedValue = 0.0f;
	_angularSpeedValue = 0.0f;
	_angle = 0.0f;
	glfwGetFramebufferSize(window, &_width, &_height);
	_startTime = (float)glfwGetTime();

	// Sets the vertices and indices arrays to draw a square on the xz plane
	GLfloat aux[] = {
		-0.577350f, 0.0f, -0.577350f,
		-0.577350f, 0.0f, 0.577350f,
		0.577350f, 0.0f, 0.577350f,
		0.577350f, 0.0f, -0.577350f };
	_vertices = new GLfloat[3 * 4];
	for (int i = 0; i < 3 * 4; i++)
		_vertices[i] = aux[i];
	_indices = new GLuint[6];
	for (int i = 0; i < 2; i++) {
		_indices[(3 * i) + 0] = 0;
		_indices[(3 * i) + 1] = i + 1;
		_indices[(3 * i) + 2] = i + 2;
	}

	// Set EBO, VAO and VBO for drawing
	glGenBuffers(1, &_EBO);
	glGenBuffers(1, &_VBO);
	glGenVertexArrays(1, &_VAO);

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 4/*number of vertices*/ * sizeof(GLfloat), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), _indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Lots of getters and setters from this point
float Plane::x() {
	return _x;
}

float Plane::y() {
	return _y;
}

float Plane::z() {
	return _z;
}

void Plane::setPosition(float x, float y, float z) {
	_x = x;
	_y = y;
	_z = z;
}

void Plane::setPosition(glm::vec3 position) {
	_x = position.x;
	_y = position.y;
	_z = position.z;
}

void Plane::setSpeed(glm::vec3 direction, float value) {
	_speedValue = value;
	// The direction vector is normalized before calculating
	*_speed = glm::normalize(direction) * _speedValue;
}

void Plane::setSpeed(float value) {
	_speedValue = value;
	*_speed = glm::normalize(*_speed) * _speedValue;
}

void Plane::setAngle(float value) {
	_angle = value;
}

void Plane::setAngularSpeed(float value) {
	_angularSpeedValue = value;
}

void Plane::setShaderProgram(GLuint *shaderProgram) {
	_shaderProgram = shaderProgram;
}

void Plane::Update() {
	// The aux vector is used to apply transformations more easily
	glm::vec4 aux = glm::vec4(_x, _y, _z, 1.0f);
	glm::mat4 transform;
	// Gets the current time and applies transform based on deltaTime between updates
	_currentTime = (float)glfwGetTime();
	float deltaTime = _currentTime - _startTime;
	_angle += (_angularSpeedValue * deltaTime);
	if (_angle > 360.0f)
		_angle -= 360.0f;

	// Sees how much the object should move
	transform = glm::translate(transform, ((*_speed) * deltaTime));
	// Saves the time of the current update
	_startTime = _currentTime;
	// Applies the translation
	aux = transform * aux;
	// Updates coordinates
	_x = aux.x;
	_y = aux.y;
	_z = aux.z;
}

void Plane::Draw() {
	// Sets shader program to be used
	glUseProgram(*_shaderProgram);

	// model transform changes from local space to world space, view changes from world to view space
	// and projection changes from view space to clip space. glTransform contains the resulting transform matrix
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 gltransform;
	// the translation moves the Plane to it's location in world space
	model = glm::translate(model, glm::vec3(_x, _y, _z));
	// the rotation applies the current angle rotation
	model = glm::rotate(model, glm::radians(_angle), *_rotationAxis);
	// the scale applies the Plane radius
	model = glm::scale(model, glm::vec3(_radius, _radius, _radius));
	// Gets the view matrix from the camera
	view = _camera->view;
	// Gets the projection matrix from the camera
	projection = _camera->projection;
	// Passes the resulting transform matrix to the vertex shader
	gltransform = projection * view * model;

	// Applies transform to shader program
	GLuint transformLoc = glGetUniformLocation(*_shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(gltransform));

	// Draws the object
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, _indices);
	glBindVertexArray(0);
}

Plane::~Plane() {
	// Frees allocated memory
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_EBO);
	delete(_speed);
	delete(_rotationAxis);
	delete(_vertices);
	delete(_indices);
}