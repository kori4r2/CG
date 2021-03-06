#pragma once
#include "Polyhedron.hpp"

class Cube: public Polyhedron {
private:
	// Overrides the generateVertices functions to generate cube vertices
	void generateVertices() override;
public:
	// Basic constructor and destructor
	Cube(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window);
	// Destructor
	~Cube();
};