#include "Cube.hpp"

void Cube::generateVertices() {
	_vertices = new GLfloat[3 * 8];
	_indices = new GLuint[3 * (_nSidesFaces - 2) * _nFaces];
	for (int i = 0; i < 3 * 8; i++) {
		_vertices[i] = Polyhedron::cubeVertices[i];
	}
	for (int i = 0; i < _nFaces; i++) {
		for (int j = 0; j < (_nSidesFaces - 2); j++) {
			_indices[(i * 3 * (_nSidesFaces - 2)) + (j * 3) + 0] = Polyhedron::cubeIndices[(i * (2 + _nSidesFaces - 2)) + j + 0];
			_indices[(i * 3 * (_nSidesFaces - 2)) + (j * 3) + 1] = Polyhedron::cubeIndices[(i * (2 + _nSidesFaces - 2)) + j + 1];
			_indices[(i * 3 * (_nSidesFaces - 2)) + (j * 3) + 2] = Polyhedron::cubeIndices[(i * (2 + _nSidesFaces - 2)) + j + 2];
		}
	}
	std::cout << "vertices:" << std::endl;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++)
			std::cout << _vertices[(3 * i) + j] << ",";
		std::cout << std::endl;
	}
	std::cout << "indices:" << std::endl;
	for (int i = 0; i < ((_nSidesFaces - 2) * _nFaces); i++) {
		for (int j = 0; j < 3; j++)
			std::cout << _indices[(3 * i) + j] << ",";
		std::cout << std::endl;
	}
}
Cube::Cube(float x, float y, float z, float radius, GLFWwindow *window, Camera *camera) 
	: Polyhedron(x, y, z,  radius, window, camera) {
	_nFaces = 6;
	_nSidesFaces = 4;
	generateVertices();
	// Set EBO, VAO and VBO for drawing
	glGenBuffers(1, &_EBO);
	glGenBuffers(1, &_VBO);
	glGenVertexArrays(1, &_VAO);

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * 8/*numero de vertices*/ * sizeof(GLfloat), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * (_nSidesFaces -2) * _nFaces * sizeof(GLuint), _indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
Cube::~Cube(){
}