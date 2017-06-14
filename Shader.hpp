#pragma once

#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

enum ShaderType {
	LIGHT,
	MATERIAL
};

enum Materials {
	NONE,
	METAL,
	PLASTIC,
	GLASS
};

enum LightType {
	NONE,
	DIRECTIONAL,
	POINT,
	SPOTLIGHT
};

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

typedef struct lightsource {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 colorRGB;
	LightType type;
}LightSource;

class Shader {
private:
	static std::vector<LightSource> _lightSources;
	GLuint _shaderProgram;
	Material _material;
public:
	Shader();
	~Shader();
};