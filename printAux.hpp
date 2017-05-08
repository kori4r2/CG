#pragma once
#include <iostream>
#include <sstream>
#include <ostream>
#include <glm\glm.hpp>
 std::string stringVec3(glm::vec3 vector) {
	 std::ostringstream stream;
	 stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
	 std::string str = stream.str();
	 return str;
}
