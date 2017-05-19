#include "Sphere.hpp"

void Sphere::generateVertices() {

	_vertices = new GLfloat[3 * _rings * _sectors];
	_indices = new GLuint[(_rings - 1) * _sectors * 4];

	for(int r = 0; r < _rings; r++){
		for (int s = 0; s < _sectors; s++) {
			// Determines height (goes from -1 to 1)
			float y = sin( ((-0.5f) * PI) + (PI * (r / (float)(_rings - 1))) );
			// sin( PI * (r/(_rings-1)) ) determines the radius of the xz plane circle at the current height
			// uses cos and sin( 2*PI * (s * (_sectors-1)) ) to go through the xz plane circle counterclockwise
			float x = cos( (2 * PI * (s / (float)(_sectors)) ) ) * sin( PI * (r / (float)(_rings - 1)) );
			float z = sin( (2 * PI * (s / (float)(_sectors)) ) ) * sin( PI * (r / (float)(_rings - 1)) );
			// Saves the vertix coordinates
			_vertices[(r * (3 *_sectors)) + (3 * s) + 0] = x;
			_vertices[(r * (3 *_sectors)) + (3 * s) + 1] = y;
			_vertices[(r * (3 *_sectors)) + (3 * s) + 2] = z;
		}
	}

	// Use a loop that goes through the vertices in the same order to fill the indices using (r, s), (r, s+1), (r+1, s+1) and (r+1, s+1) vertices for each square
	for (int r = 0; r < (_rings - 1); r++) {
		for (int s = 0; s < _sectors; s++) {
			_indices[(r * (4 *_sectors)) + (s * 4) + 0] = (r * _sectors) + s;
			// The module is used to get the first sector when the end is reached
			_indices[(r * (4 * _sectors)) + (s * 4) + 1] = (r * _sectors) + ((s + 1) % _sectors);
			_indices[(r * (4 * _sectors)) + (s * 4) + 2] = ((r + 1) * _sectors) + ((s + 1) % _sectors);
			_indices[(r * (4 * _sectors)) + (s * 4) + 3] = ((r + 1) * _sectors) + s;
		}
	}
}

Sphere::Sphere(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window)
	// Calls base class constructor
	: Polyhedron(x, y, z, radius, camera, window){
	// Sets variables that weren't set by base constructor
	_nFaces = 1;
	_nSidesFaces = 0;
	// Determines how segmented the sphere will be, higher values have smoother but heavier spheres
	_sectors = 15;
	_rings = 15;
	// Generates vertices
	generateVertices();
	// Set EBO, VAO and VBO for drawing
	glGenBuffers(1, &_EBO);
	glGenBuffers(1, &_VBO);
	glGenVertexArrays(1, &_VAO);

	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * _rings * _sectors/*number of vertices*/ * sizeof(GLfloat), _vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * (_rings - 1) * _sectors * sizeof(GLuint), _indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sphere::Draw() {
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
	glDrawElements(GL_QUADS, 4 * _sectors * (_rings - 1), GL_UNSIGNED_INT, _indices);
	glBindVertexArray(0);
}

Sphere::~Sphere() {
	// Nothing to add to base class destructor
}