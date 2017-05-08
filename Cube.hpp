#pragma once
#include "Polyhedron.hpp"

class Cube: public Polyhedron {
private:
	void generateVertices() override;
public:
	Cube(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window);
	~Cube();
};
