#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

enum ReflectionModel {
	PHONG = 0,
	GOURAND,
	FLAT
};

enum ShaderType {
	LIGHT,
	MATERIAL
};

enum Materials {
	DEFAULT,
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

class Material {
private:
	glm::vec3 _colorRGB, _ambient, _diffuse, _specular;
	float _shininess, _transparency;
public:
	const glm::vec3 &colorRGB, &ambient, &diffuse, &specular;
	const float &shininess, &transparency;

	Material();
	Material(glm::vec3 color);
	Material(Materials type, glm::vec3 color);
	Material& operator=(const Material &original);
	~Material();
};

typedef struct my_lightsource {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 colorRGB;
	LightType type;
}LightSource;

class Shader {
private:
	static std::vector<LightSource> _lightSources;
	static int _lightCount;
	static GLuint _shaderProgram[3];
	static bool _programsCompiled;
	ShaderType _type;
	int _lightIndex;

	GLuint linkProgram(const char *vShaderFilename, const char *fShaderFilename, GLint *success);

public:
	static ReflectionModel reflectionModel;

	Shader();
	void makeLightSource(LightType lightType, glm::vec3 position, glm::vec3 color, glm::vec3 direction);
	void Use(Material material, glm::vec3 position, glm::mat4 projection, glm::mat4 view, glm::mat4 model);
	~Shader();
};