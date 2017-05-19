#pragma once
#include "Polyhedron.hpp"

class Tetrahedron : public Polyhedron {
private:
	// Overrides the generateVertices functions to generate tetrahedron vertices
	void generateVertices() override;
public:
	// Basic constructor and destructor
	Tetrahedron(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window);
	~Tetrahedron();
};
