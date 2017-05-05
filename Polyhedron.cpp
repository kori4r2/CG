#include "Polyhedron.hpp"
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

Polyhedron::Polyhedron(float x, float y, float z, float radius, GLFWwindow *window, Camera *camera){
	_camera = camera;
	_x = x;
	_y = y;
	_z = z;
	_radius = radius;
	_speedDirection = new glm::vec3(0.0f, 0.0f, 0.0f);
	_rotationAxis = new glm::vec3(0.0f, 1.0f, 0.0f);
	_speedValue = 0;
	_angularSpeedValue = 0;
	_angle = 0;
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
void Polyhedron::setSpeed(glm::vec3 direction, float value) {
	_speedValue = value;
	*_speedDirection = glm::normalize(direction);
}
void Polyhedron::setSpeed(float value) {
	_speedValue = value;
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
	_currentTime = (GLfloat)glfwGetTime();
	_angle += (_angularSpeedValue * (_currentTime - _startTime));
	transform = glm::translate(transform, ((*_speedDirection) * _speedValue) * (_currentTime - _startTime));
	// Saves the time of the current update
	_startTime = _currentTime;
	// Applies the translation and updates current position
	aux = transform * aux;
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
	glm::vec3 glSpeed = (*_speedDirection) * _speedValue;
	// the translation moves the polyhedron to it's location in world space
	model = glm::translate(model, glm::vec3(_x, _y, _z));
	model = glm::rotate(model, _angle, *_rotationAxis);
	// the scale applies the polyhedron radius
	model = glm::scale(model, glm::vec3(_radius, _radius, _radius));
	// moves the camera back a bit
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	view = _camera->view;
	// applies orthogonal view
	projection = glm::perspective(_camera->fov, (float)_width / (float)_height, 0.1f, 1000.0f);
	//projection = glm::ortho(0.0f, (float)_width, 0.0f, (float)_height, 0.0f, 1000.0f);
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
	delete(_speedDirection);
	delete(_rotationAxis);
	delete(_vertices);
	delete(_indices);
}