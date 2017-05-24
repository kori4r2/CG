#pragma once
#include "Polyhedron.hpp"

#define PI 3.14159265359

class Sphere : public Polyhedron {
protected:
	// Each sector is a semi-circle from north pole to south pole
	// Earch ring is a full circle around the pole axis at a different latitude
	// _sectors and _rings will store the number of sectors and rings dividing the sphere
	const int _sectors, _rings;

	// Overrides the generateVertices functions to generate sphere vertices
	void generateVertices() override;
	// Constructor that allows child classes to decide number of rings and sectors, creating different polyhedrons
	Sphere(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window, int nRings, int nSectors);

public:
	// Basic constructor
	Sphere(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window);
	// Destructor
	~Sphere();
};