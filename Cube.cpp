#include "Cube.hpp"

void Cube::generateVertices() {
	// Creates vertices and indices arrays
	_vertices = new GLfloat[3 * 8];
	_indices = new GLuint[_nSidesFaces * _nFaces];
	// Obtains all vertices from the static variable set by base class
	for (int i = 0; i < 3 * 8; i++) {
		_vertices[i] = Polyhedron::cubeVertices[i];
	}
	// Obtains indices from the static variable set by base class
	// Each line on the static variable corresponds to the vertices of a face
	for (int i = 0; i < _nFaces; i++) {
		_indices[(_nSidesFaces * i) + 0] = Polyhedron::cubeIndices[(_nSidesFaces * i) + 0];
		_indices[(_nSidesFaces * i) + 1] = Polyhedron::cubeIndices[(_nSidesFaces * i) + 1];
		_indices[(_nSidesFaces * i) + 2] = Polyhedron::cubeIndices[(_nSidesFaces * i) + 2];
		_indices[(_nSidesFaces * i) + 3] = Polyhedron::cubeIndices[(_nSidesFaces * i) + 3];
	}
}

Cube::Cube(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window)
	// Calls base class constructor
	: Polyhedron(x, y, z,  radius, camera, window) {
	// Sets variables that weren't set by base constructor
	_nFaces = 6;
	_nSidesFaces = 4;
	// Generates vertices
	generateVertices();
	// Set EBO, VAO and VBO for drawing
	glGenBuffers(1, &_EBO);
	glGenBuffers(1, &_VBO);
	glGenVertexArrays(1, &_VAO);

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 8/*number of vertices*/ * sizeof(GLfloat), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _nSidesFaces * _nFaces * sizeof(GLuint), _indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Pretty much equal to the standard draw function
void Cube::Draw() {
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
	model = glm::rotate(model, (float)_angle, *_rotationAxis);
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
	glBindVertexArray(_VAO);
	// Draws using quads instead of triangles
	glDrawElements(GL_QUADS, _nFaces * _nSidesFaces, GL_UNSIGNED_INT, _indices);
	glBindVertexArray(0);
}

Cube::~Cube(){
	// Nothing to add to base class destructor
}