#include "Polyhedron.hpp"

const GLfloat Polyhedron::cubeVertices[] = {
	-0.577350f, -0.577350f, -0.577350f,
	-0.577350f, -0.577350f, 0.577350f,
	-0.577350f, 0.577350f, -0.577350f,
	-0.577350f, 0.577350f, 0.577350f,
	0.577350f, -0.577350f, -0.577350f,
	0.577350f, -0.577350f, 0.577350f,
	0.577350f, 0.577350f, -0.577350f,
	0.577350f, 0.577350f, 0.577350f
};

const GLfloat Polyhedron::cubeIndices[] = {
	0, 4, 5, 1,
	1, 5, 7, 3,
	3, 2, 0, 1,
	7, 6, 4, 5,
	4, 5, 7, 6,
	2, 6, 4, 0
};

const GLfloat Polyhedron::tetrahedronVertices[] = {
	0.0f, 1.0f, 0.0f,
	0.0f, -0.333333f, 0.9428090f,
	-0.816497f, -0.333333f, -0.471405f,
	0.816497f, -0.333333f, -0.471405f
};

const GLfloat Polyhedron::tetrahedronIndices[] = {
	0, 1, 2, 3, 0, 1 // each 3 consecutive vertices in this line should draw a triangle
};

Polyhedron::Polyhedron(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window){
	// Gets variables ready
	_camera = camera;
	_hasGravity = false;
	_x = x;
	_y = y;
	_z = z;
	_radius = radius;
	_gravityValue = 200.0f;
	_speed = new glm::vec3(0.0f, 0.0f, 0.0f);
	_rotationAxis = new glm::vec3(0.0f, 1.0f, 0.0f);
	_gravity = new glm::vec3(0.0f, -1.0f, 0.0f);
	_speedValue = 0.0f;
	_angularSpeedValue = 0.0f;
	_angle = 0.0f;
	glfwGetFramebufferSize(window, &_width, &_height);
	_startTime = (float)glfwGetTime();
}

// Lots of getters and setters from this point
float Polyhedron::x() {
	return _x;
}

float Polyhedron::y() {
	return _y;
}

float Polyhedron::z() {
	return _z;
}

void Polyhedron::setPosition(float x, float y, float z) {
	_x = x;
	_y = y;
	_z = z;
}

void Polyhedron::setGravity(float value) {
	_gravityValue = value;
}

void Polyhedron::enableGravity() {
	_hasGravity = true;
}

void Polyhedron::disableGravity() {
	_hasGravity = false;
}

void Polyhedron::setPosition(glm::vec3 position) {
	_x = position.x;
	_y = position.y;
	_z = position.z;
}

void Polyhedron::setSpeed(glm::vec3 direction, float value) {
	_speedValue = value;
	// The direction vector is normalized before calculating
	*_speed = glm::normalize(direction) * _speedValue;
}

void Polyhedron::setSpeed(float value) {
	// This function doesn't work if the object is not moving
	if (_speedValue != 0)
		*_speed = glm::normalize(*_speed);
	_speedValue = value;
	*_speed = *_speed * _speedValue;
}

void Polyhedron::setAngle(float value) {
	_angle = value;
}

void Polyhedron::setRadius(float value) {
	_radius = value;
}

void Polyhedron::setAngularSpeed(glm::vec3 axis, float value) {
	// The axis vector is normalized before being stored
	*_rotationAxis = glm::normalize(axis);
	_angularSpeedValue = value;
}

void Polyhedron::setAngularSpeed(float value) {
	_angularSpeedValue = value;
}

void Polyhedron::setShaderProgram(GLuint *shaderProgram) {
	_shaderProgram = shaderProgram;
}

void Polyhedron::Update() {
	// The aux vector is used to apply transformations more easily
	glm::vec4 aux = glm::vec4(_x, _y, _z, 1.0f);
	glm::mat4 transform;
	// Gets the current time and applies transform based on deltaTime between updates
	_currentTime = (float)glfwGetTime();
	float deltaTime = _currentTime - _startTime;
	_angle += (_angularSpeedValue * deltaTime);
	if (_angle > 360.0f)
		_angle -= 360.0f;

	glm::vec3 gravityEffect = (*_gravity) * _gravityValue * deltaTime;

	// IF the object is affected by gravity, calculates the new speed
	if (_hasGravity) {
		(*_speed) += gravityEffect;
		_speedValue = glm::length(*_speed);
	}

	// Sees how much the object should move
	transform = glm::translate(transform, ((*_speed) * deltaTime));
	// Saves the time of the current update
	_startTime = _currentTime;
	// Applies the translation
	aux = transform * aux;
	// If the object goes below ground level (y = 0), makes it go back and stop y axis movement
	if ((aux.y - _radius) <= 0.0f) {
		aux.y = _radius;
		_speed->y = 0.0f;
		_speedValue = glm::length(*_speed);
	}
	// Updates coordinates
	_x = aux.x;
	_y = aux.y;
	_z = aux.z;
}

void Polyhedron::Draw() {
	// Sets shader program to be used
	glUseProgram(*_shaderProgram);

	// model transform changes from local space to world space, view changes from world to view space
	// and projection changes from view space to clip space. glTransform contains the resulting transform matrix
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 gltransform;
	// the translation moves the polyhedron to it's location in world space
	model = glm::translate(model, glm::vec3(_x, _y, _z));
	// the rotation applies the current angle rotation
	model = glm::rotate(model, glm::radians(_angle), *_rotationAxis);
	// the scale applies the polyhedron radius
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 3 * _nFaces * (_nSidesFaces - 2), GL_UNSIGNED_INT, _indices);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Polyhedron::~Polyhedron() {
	// Frees allocated memory
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_EBO);
	delete(_speed);
	delete(_rotationAxis);
	delete(_vertices);
	delete(_indices);
	delete(_gravity);
}