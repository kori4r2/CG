#pragma once
#include <math.h>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.hpp"
#include "Shader.hpp"

class Polyhedron {
private:
	// Each polyhedron has a different method of generating vertices
	virtual void generateVertices() = 0;

protected:
	// These static variables are here to help when implementing the generateVertices functions of some polyhedrons
	static const GLfloat cubeVertices[], tetrahedronVertices[];
	static const GLuint cubeIndices[], tetrahedronIndices[];
	// Determines wether the object should be affected by gravity
	bool _hasGravity;
	// _x, _y and _z are the world space coordinates, _radius is the object radius, _speedValue and _angularSpeedValue
	// are the absolute values of the respecive speeds, _angle is the current rotated value, _gravityValue is self explanatory
	// _startTime and _currentTime are used for updates
	float _x, _y, _z, _radius, _speedValue, _angularSpeedValue, _angle, _gravityValue, _startTime, _currentTime;
	// _width and _height are window size parameters, _nfaces is how many faces the polyhedron has, _nSidesFaces is how many
	// sides each face of the polyhedron has
	int _width, _height, _nFaces, _nSidesFaces;
	// _speed contains the current speed of the object(not just the direction), _rotationAxis is self explanatory, _gravity
	// contains the direction of the gravity force
	glm::vec3 *_speed, *_rotationAxis, *_gravity;
	// Variables needed for drawing
	GLfloat *_vertices;
	GLuint *_indices;
	GLuint _VAO, _VBO, _EBO;
	// A reference to the camera object
	Camera *_camera;
	// This object's material and shader
	Material _material;
	Shader _shader;

public:
	// Pretty simple constructor
	Polyhedron(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window);
	// Some getters and setters
	float x();
	float y();
	float z();
	void setGravity(float value);
	void enableGravity();
	void disableGravity();
	void setPosition(float x, float y, float z);
	void setPosition(glm::vec3 position);
	void setSpeed(glm::vec3 direction, float value);
	void setSpeed(float value);
	void setAngle(float value);
	void setRadius(float value);
	void setAngularSpeed(glm::vec3 axis, float value);
	void setAngularSpeed(float value);
	void setMaterial(Material material);
	// Update and draw
	void Update();
	void Draw();
	//Destructor
	~Polyhedron();
};