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

// Maximum number of light sources allowed
#define MAX_N_LIGHTS 8

// Enums used to make the code more understandable
enum ReflectionModel {
	PHONG = 0,
	GOURAUD,
	FLAT
};

enum ShaderType {
	LIGHT,
	MATERIAL
};

// These were originally another enum but to avoid problems with the shaders they have been changed to const int
const int DEFAULT = 0;
const int METAL = 1;
const int PLASTIC = 2;
const int GLASS = 3;
const int LIGHTBALL = 4;
typedef int Materials;

const int NONE = 0;
const int DIRECTIONAL = 1;
const int POINT = 2;
const int SPOTLIGHT = 3;
typedef int LightType;


// The class material has all needed information for calculating the effect of light on an object
// It was not created on a separate file because the shaders won't work without materials defined
class Material {
private:
	// Variables to calculate the color of this object when affected by light
	glm::vec3 _colorRGB, _ambient, _diffuse, _specular;
	float _shininess, _transparency;
	Materials _type;
public:
	// const references to the variables, so they can be accessed from outside, but not altered
	const glm::vec3 &colorRGB, &ambient, &diffuse, &specular;
	const float &shininess, &transparency;
	const Materials &type;

	// Basic constructor with default gray color
	Material();
	// Constructor with default material and desired color
	Material(glm::vec3 color);
	// Constructor with desired material and color
	Material(Materials type, glm::vec3 color);
	// This was necessary, for some reason it said the function was deleted (OH, WELL)
	Material& operator=(const Material &original);
	// Basic destructor
	~Material();
};

// A struct that stores all needed variables of any type of light source
typedef struct my_lightsource {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 colorRGB;
	float cutOff;
	float outerCutOff;
	float constantParameter;
	float linearParameter;
	float quadraticParameter;
	LightType type;
}LightSource;

class Shader {
private:
	// A vector containing all the light sources created
	static std::vector<LightSource> _lightSources;
	// A counter of how many light sources currently exist
	static int _lightCount;
	// A vector to all shader programs availabe (FLAT SHADING IS NOT IMPLEMENTED)
	static GLuint _shaderProgram[3];
	// A boolean that determines if all the programs are already compiled
	static bool _programsCompiled;
	// The type of the current shader
	ShaderType _type;
	// The index of this light source (-1 if this object is not a light source)
	int _lightIndex;

	// Private function to link a shader program, makes things easier to read
	GLuint linkProgram(const char *vShaderFilename, const char *fShaderFilename, GLint *success);

public:
	// This variable determines the reflection model utilized
	static ReflectionModel reflectionModel;

	// Basic constructor
	Shader();
	// Turns the object with the current shader into a lightsource with the desired settings
	// This functions creates a directional light
	bool makeLightSource(glm::vec3 color, glm::vec3 direction);
	// This one for point light
	bool makeLightSource(glm::vec3 color, float constant, float  linear, float  quadratic);
	// This one for spotlight
	bool makeLightSource(glm::vec3 color, glm::vec3 direction, float constant, float  linear, float  quadratic, float cutOff, float outerCutOff);
	// Uses the current shader, passing the necessary parameters to the GPU pipeline
	void Use(Material material, glm::vec3 viewPos,glm::mat4 projection, glm::mat4 view, glm::mat4 model);
	// Updates position, essential for light source calculations
	void Update(glm::vec3 position);
	~Shader();
};