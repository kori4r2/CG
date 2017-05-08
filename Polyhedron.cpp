#include "Polyhedron.hpp"
#include "printAux.hpp"
const GLfloat Polyhedron::cubeVertices[] = {
	-0.306186f, -0.306186f, -0.306186f,
	-0.306186f, -0.306186f, 0.306186f,
	-0.306186f, 0.306186f, -0.306186f,
	-0.306186f, 0.306186f, 0.306186f,
	0.306186f, -0.306186f, -0.306186f,
	0.306186f, -0.306186f, 0.306186f,
	0.306186f, 0.306186f, -0.306186f,
	0.306186f, 0.306186f, 0.306186f
};
const GLfloat Polyhedron::cubeIndices[] = {
	0, 4, 1, 5, // each 3 consecutive vertices in this line should draw a triangle
	1, 5, 3, 7,
	3, 2, 1, 0,
	7, 6, 5, 4,
	0, 4, 1, 5,
	2, 6, 0, 4
};
const GLfloat Polyhedron::tetrahedronVertices[] = {
	0.0f, 1.0f, 0.0f,
	0.0f, -0.333333f, 0.9428090f,
	-0.816497, -0.333333f, -0.471405,
	0.816497, -0.333333f, -0.471405
};
const GLfloat Polyhedron::tetrahedronIndices[] = {
	0, 1, 2, 3, 0, 1 // each 3 consecutive vertices in this line should draw a triangle
};

Polyhedron::Polyhedron(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window){
	_camera = camera;
	_hasGravity = false;
	_x = x;
	_y = y;
	_z = z;
	_radius = radius;
	_gravityValue = 150.0f;
	_speed = new glm::vec3(0.0f, 0.0f, 0.0f);
	_rotationAxis = new glm::vec3(0.0f, 1.0f, 0.0f);
	_gravity = new glm::vec3(0.0f, -1.0f, 0.0f);
	_speedValue = 0.0f;
	_angularSpeedValue = 0.0f;
	_angle = 0.0f;
	glfwGetFramebufferSize(window, &_width, &_height);
	_startTime = (float)glfwGetTime();
}
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
	*_speed = glm::normalize(direction) * _speedValue;
}
void Polyhedron::setSpeed(float value) {
	_speedValue = value;
	*_speed = glm::normalize(*_speed) * _speedValue;
}
void Polyhedron::setAngle(float value) {
	_angle = value;
}
void Polyhedron::setAngularSpeed(glm::vec3 axis, float value) {
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
	glm::vec4 aux = glm::vec4(_x, _y, _z, 1.0f);
	glm::mat4 transform;
	// Gets the current time and applies transform based on deltaTime between updates
	_currentTime = (float)glfwGetTime();
	float deltaTime = _currentTime - _startTime;
	_angle += (_angularSpeedValue * deltaTime);
	glm::vec3 gravity = (*_gravity) * _gravityValue * deltaTime;

	if (_hasGravity) {
		(*_speed) += gravity;
		_speedValue = _speed->length();
	}

	transform = glm::translate(transform, ((*_speed) * deltaTime));
	// Saves the time of the current update
	_startTime = _currentTime;
	// Applies the translation and updates current position
	aux = transform * aux;
	if (aux.y <= 0.0f) {
		aux.y = 0.0f;
		_speed->y = 0.0f;
		_speedValue = _speed->length();
	}
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
	model = glm::rotate(model, (float)_angle, *_rotationAxis);
	// the scale applies the polyhedron radius
	model = glm::scale(model, glm::vec3(_radius, _radius, _radius));
	// Gets the view matrix from the camera
	view = _camera->view;
	// applies perspective view using the camera parameters
	projection = _camera->projection;
	// Passes the resulting transform matrix to the vertex shader
	gltransform = projection * view * model;

	// Applies transform to shader program
	GLuint transformLoc = glGetUniformLocation(*_shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(gltransform));

	// Draws the object, no surprises
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 3 * _nFaces * (_nSidesFaces - 2), GL_UNSIGNED_INT, _indices);
	glBindVertexArray(0);
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