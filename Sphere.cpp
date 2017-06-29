#include "Sphere.hpp"

void Sphere::generateVertices() {

	_vertices = new GLfloat[3 * (_rings * _sectors)];
	_indices = new GLuint[3 * (_nSidesFaces - 2) * _nFaces];

	for(int r = 0; r < _rings; r++){
		for (int s = 0; s < _sectors; s++) {
			// Determines height (goes from -1 to 1)
			GLfloat y = (GLfloat)sin( ((-0.5f) * PI) + (PI * (r / (float)(_rings - 1))) );
			// sin( PI * (r/(_rings-1)) ) determines the radius of the xz plane circle at the current height
			// Uses cos and sin( 2*PI * (s * (_sectors-1)) ) to go through the xz plane circle counterclockwise,
			// generating evenly spaced vertices, one for each sector
			GLfloat x = (GLfloat)cos( (2 * PI * (s / (float)(_sectors)) ) ) * sin( PI * (r / (float)(_rings - 1)) );
			GLfloat z = (GLfloat)sin( (2 * PI * (s / (float)(_sectors)) ) ) * sin( PI * (r / (float)(_rings - 1)) );
			// Saves the vertix coordinates
			_vertices[(r * (3 *_sectors)) + (3 * s) + 0] = x;
			_vertices[(r * (3 *_sectors)) + (3 * s) + 1] = y;
			_vertices[(r * (3 *_sectors)) + (3 * s) + 2] = z;
		}
	}

	// Use a loop that goes through the vertices in the same order to fill the indices using (r, s), (r, s+1), (r+1, s+1) and (r+1, s+1) vertices for each square
	for (int r = 0; r < (_rings - 1); r++) {
		for (int s = 0; s < _sectors; s++) {
			// Gets indices to draw 2 triangles to form the current square
			_indices[(r * (6 *_sectors)) + (s * 6) + 0] = (r * _sectors) + s;
			// The module is used to get the first sector when the end is reached
			_indices[(r * (6 * _sectors)) + (s * 6) + 1] = (r * _sectors) + ((s + 1) % _sectors);
			_indices[(r * (6 * _sectors)) + (s * 6) + 2] = ((r + 1) * _sectors) + ((s + 1) % _sectors);
			// Second triangle
			_indices[(r * (6 * _sectors)) + (s * 6) + 3] = (r * _sectors) + s;
			_indices[(r * (6 * _sectors)) + (s * 6) + 4] = ((r + 1) * _sectors) + s;
			_indices[(r * (6 * _sectors)) + (s * 6) + 5] = ((r + 1) * _sectors) + ((s + 1) % _sectors);
		}
	}
}

Sphere::Sphere(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window, int nRings, int nSectors)
// Calls base class constructor
	: Polyhedron(x, y, z, radius, camera, window),
	// Determines how segmented the sphere will be, higher values have smoother but heavier to load spheres
	_sectors(nSectors), _rings(nRings) {
	// Sets variables that weren't set by base constructor
	_nFaces = _sectors * (_rings - 1);
	_nSidesFaces = 4;
	// Generates vertices
	generateVertices();
	// Set EBO, VAO and VBO for drawing
	glGenBuffers(1, &_EBO);
	glGenBuffers(1, &_VBO);
	glGenVertexArrays(1, &_VAO);

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * (_rings * _sectors)/*number of vertices*/ * sizeof(GLfloat), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * (_rings - 1) * _sectors * sizeof(GLuint), _indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Sphere::Sphere(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window)
	// Calls base class constructor
	: Polyhedron(x, y, z, radius, camera, window),
	// Sets default values for number of rings and number of sectors
	_sectors(15), _rings(15){
	// Sets variables that weren't set by base constructor
	_nFaces = _sectors * (_rings-1);
	_nSidesFaces = 4;
	// Generates vertices
	generateVertices();
	// Set EBO, VAO and VBO for drawing
	glGenBuffers(1, &_EBO);
	glGenBuffers(1, &_VBO);
	glGenVertexArrays(1, &_VAO);

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * (_rings * _sectors)/*number of vertices*/ * sizeof(GLfloat), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * (_rings - 1) * _sectors * sizeof(GLuint), _indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Sphere::~Sphere() {
	// Nothing to add to base class destructor
}