#pragma once
#include "Polyhedron.hpp"

#define PI 3.14159265359

class Sphere : public Polyhedron {
private:
	// Overrides the generateVertices functions to generate cube vertices
	void generateVertices() override;
	// Each sector is a semi-circle from north pole to south pole
	// Earch ring is a full circle around the pole axis at a different latitude
	// _sectors and _rings will store the number of sectors and rings dividing the sphere
	int _sectors, _rings;

public:
	// Basic constructor and destructor
	void Draw();
	Sphere(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window);
	~Sphere();
};