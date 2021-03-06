#include "Cube.hpp"

void Cube::generateVertices() {
	// Creates vertices and indices arrays
	_vertices = new GLfloat[3 * 8];
	_indices = new GLuint[3 * (_nSidesFaces - 2) * _nFaces];
	// Obtains all vertices from the static variable set by base class
	for (int i = 0; i < 3 * 8; i++) {
		_vertices[i] = Polyhedron::cubeVertices[i];
	}
	// Obtains indices from the static variable set by base class
	// Each line on the static variable corresponds to the vertices of a face
	for (int i = 0; i < _nFaces; i++) {
		for (int j = 0; j < (_nSidesFaces - 2); j++) {
			_indices[(3 * (_nSidesFaces - 2) * i) + (j * 3) + 0] = Polyhedron::cubeIndices[(_nSidesFaces * i) + 0];
			_indices[(3 * (_nSidesFaces - 2) * i) + (j * 3) + 1] = Polyhedron::cubeIndices[(_nSidesFaces * i) + j + 1];
			_indices[(3 * (_nSidesFaces - 2) * i) + (j * 3) + 2] = Polyhedron::cubeIndices[(_nSidesFaces * i) + j + 2];
		}
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * (_nSidesFaces - 2) * _nFaces * sizeof(GLuint), _indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Cube::~Cube(){
	// Nothing to add to base class destructor
}