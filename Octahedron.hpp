#pragma once
#include "Sphere.hpp"

class Octahedron : public Sphere{
public:
	// Basic constructor
	Octahedron(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window);
	// Destructor
	~Octahedron();
};
