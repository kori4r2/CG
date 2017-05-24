#include "Octahedron.hpp"

Octahedron::Octahedron(float x, float y, float z, float radius, Camera *camera, GLFWwindow *window)
	: Sphere(x, y, z, radius, camera, window, 3, 4){
}
// Destructor
Octahedron::~Octahedron() {
	// Nothing to add to base class destructor
}