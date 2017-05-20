#include "Tetrahedron.hpp"

void Tetrahedron::generateVertices() {
	// Creates vertices and indices arrays
	_vertices = new GLfloat[3 * 4];
	_indices = new GLuint[_nSidesFaces * _nFaces];
	// Obtains all vertices from the static variable set by base class
	for (int i = 0; i < 3 * 4; i++) {
		_vertices[i] = Polyhedron::tetrahedronVertices[i];
	}
	// Obtains indices from the static variable set by base class
	// Every 3 consecutive vertices in the line correspond to the vertices that form a triangle
	for (int i = 0; i < (_nFaces * (_nSidesFaces - 2)); i++) {
		_indices[(i * 3) + 0] = Polyhedron::tetrahedronIndices[i];
		_indices[(i * 3) + 1] = Polyhedron::tetrahedronIndices[i + 1];
		_indices[(i * 3) + 2] = Polyhedron::tetrahedronIndices[i + 2];
	}
}

Tetrahedron::Tetrahedron(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window)
	// Calls base class constructor
	: Polyhedron(x, y, z, radius, camera, window){
	// Sets variables that weren't set by base constructor
	_nFaces = 4;
	_nSidesFaces = 3;
	// Generates vertices
	generateVertices();
	// Set EBO, VAO and VBO for drawing
	glGenBuffers(1, &_EBO);
	glGenBuffers(1, &_VBO);
	glGenVertexArrays(1, &_VAO);

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 4/*number of vertices*/ * sizeof(GLfloat), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * (_nSidesFaces - 2) * _nFaces * sizeof(GLuint), _indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Tetrahedron::~Tetrahedron() {
	// Nothing to add to base class destructor
}