#include "Shader.hpp"

// Basic constructor, creates a default material that alters absolutely nothing and has gray color
Material::Material() 
	: colorRGB(_colorRGB), ambient(_ambient), diffuse(_diffuse), specular(_specular),
	shininess(_shininess), transparency(_transparency){
	_ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	_shininess = 1.0f;
	_transparency = 1.0f;
	_colorRGB = glm::vec3(0.5f, 0.5f, 0.5f);
}

// Calls base constructor and then changes color
Material::Material(glm::vec3 color) 
	:Material(){
	_colorRGB = color;
}

// Calls color constructor and the determines the other variables base on type and color
Material::Material(Materials type, glm::vec3 color)
	: Material(color){
	switch (type) {
	case(DEFAULT):
		break;
	case(METAL):
		_ambient = glm::vec3(0.25f);
		_diffuse = glm::vec3(0.4f);
		_specular = glm::vec3(0.774597f);
		_shininess = 128.0f * 0.6f;
		_transparency = 1.0f;
		break;	
	case(PLASTIC):
		_ambient = glm::vec3(0.0f);
		_diffuse = glm::vec3(0.55f);
		_specular = glm::vec3(0.7f);
		_shininess = 128.0f * 0.25f;
		_transparency = 1.0f;
		break;
	case(GLASS):
		_ambient = glm::vec3(0.15f);
		_diffuse = glm::vec3(0.45f);
		_specular = glm::vec3(0.7f);
		_shininess = 128.0f * 0.4f;
		_transparency = 0.75f;
		break;
	case(LIGHTBALL):
		_ambient = glm::vec3(1.0f);
		_diffuse = glm::vec3(0.0f);
		_specular = glm::vec3(0.0f);
		_shininess = 0.0f;
		_transparency = 1.0f;
		break;
	}
}

// Copy assignmet was complaining
Material& Material::operator=(const Material &original) {
	_colorRGB = original.colorRGB;
	_ambient = original.ambient;
	_diffuse = original.diffuse;
	_specular = original.specular;
	_shininess = original.shininess;
	_transparency = original.transparency;
	return *this;
}

// Basic destructor
Material::~Material() {
}

// Initializations of Shader class static variables
int Shader::_lightCount = 0;
std::vector<LightSource> Shader::_lightSources = std::vector<LightSource>(0);
ReflectionModel Shader::reflectionModel = PHONG;
bool Shader::_programsCompiled = false;
GLuint Shader::_shaderProgram[3] = {0, 0, 0};

// Compiles a program with the two shader codes passed
GLuint Shader::linkProgram(const char *vShaderFilename, const char *fShaderFilename, GLint *success) {
	std::ifstream vShaderFile, fShaderFile;
	std::stringstream vShaderStream, fShaderStream;
	GLuint vertexShader, fragmentShader, shaderProgram;
	char infolog[1024];

	// Tries to open the files
	vShaderFile.open(vShaderFilename);
	fShaderFile.open(fShaderFilename);
	// In case of error, writes message to cout and returns 0
	if (vShaderFile.bad() || fShaderFile.bad()) {
		if (!vShaderFile.bad())
			vShaderFile.close();
		else
			fShaderFile.close();
		std::cout << "Failed to load shader files" << std::endl;
		return 0;
	}
	// Reads the files's contents and converts them to a const char*
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();
	vShaderFile.close();
	fShaderFile.close();
	// I have no idea why this middle steps is needed but it is and it cost me a few hours
	std::string auxString1 = vShaderStream.str();
	std::string auxString2 = fShaderStream.str();
	const char *vertexSource = auxString1.c_str();
	const char *fragmentSource = auxString2.c_str();

	// Tries to compile the vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, success);
	// In case of failure, writes the details to cout and returns 0
	if (!(*success)) {
		glGetShaderInfoLog(vertexShader, 1024, nullptr, infolog);
		std::cout << "Failed to compile vertex shader" << infolog << std::endl;
		return 0;
	}

	// Same as above, but for the fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, success);
	if (!(*success)) {
		glDeleteShader(vertexShader);
		glGetShaderInfoLog(fragmentShader, 1024, nullptr, infolog);
		std::cout << "Failed to compile fragment shader" << infolog << std::endl;
		return 0;
	}

	// Tries to link both shaders and create the shader program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, success);
	// In case of failure, writes the details to cout and returns 0
	if (!(*success)) {
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glGetProgramInfoLog(shaderProgram, 1024, nullptr, infolog);
		std::cout << "Failed to link shader program" << infolog << std::endl;
		return 0;
	}

	// Deletes unnecessary variables and returns the linked program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}


Shader::Shader() {
	// When creating a shader checks if the programs have already been compiled
	if (!_programsCompiled) {
		// If they havent, compiles all three of them, exiting on failure
		GLint success = 1;
		_shaderProgram[PHONG] = linkProgram("light_materials_phong.vs", "light_materials_phong.fs", &success);
		if (!success) {
			system("pause");
			exit(1);
		}
		_shaderProgram[GOURAUD] = linkProgram("light_materials_gouraud.vs", "light_materials_gouraud.fs", &success);
		if (!success){
			system("pause");
			exit(1);
		}
		//_shaderProgram[FLAT] = linkProgram("light_materials_flat.vs", "light_materials_flat.fs", &success);
		//if (!success){
		//	system("pause");
		//	exit(1);
		//}
		// Sets this to true, so it doesn't have to compile again
		_programsCompiled = true;
	}
	// By default, every shader created is for a material
	_type = MATERIAL;
	_lightIndex = -1;
}

void Shader::makeLightSource(glm::vec3 color, glm::vec3 direction) {
	// Checks if other light sources can be added
	if (_lightCount <= MAX_N_LIGHTS && _type != LIGHT) {
		// Marks shader as a light source, sets the parameters for a new light source as asked and updates the vector and counter
		_type = LIGHT;
		_lightIndex = _lightCount;
		_lightCount++;
		LightSource thisLight;
		thisLight.colorRGB = color;
		thisLight.direction = glm::normalize(direction);
		thisLight.type = DIRECTIONAL;
		thisLight.position = glm::vec3(0.0f, 0.0f, 0.0f);
		thisLight.constantParameter = 0.0f;
		thisLight.linearParameter = 0.0f;
		thisLight.quadraticParameter = 0.0f;
		thisLight.cutOff = 0.0f;
		thisLight.outerCutOff = 0.0f;
		_lightSources.push_back(thisLight);
		std::cout << "Successfully added light source at index " << _lightIndex << std::endl;
	}
	else if (_type == LIGHT) {
		// If the object is already a light source does nothing and lets the user know why
		// To do: change light source settings instead of doing nothing
		std::cout << "This object is already a light source" << _lightIndex << std::endl;
	} else {
		// If the limit is reached, does nothing and lets the user know why
		std::cout << "Maximum amount of light sources has already been reached" << std::endl;
	}
}
void Shader::makeLightSource(glm::vec3 color, float constant, float  linear, float  quadratic) {
	// Checks if other light sources can be added
	if (_lightCount <= MAX_N_LIGHTS) {
		// Marks shader as a light source, sets the parameters for a new light source as asked and updates the vector and counter
		makeLightSource(color, glm::vec3(0.0f, -1.0f, 0.0f));
		_lightSources[_lightIndex].constantParameter = constant;
		_lightSources[_lightIndex].linearParameter = linear;
		_lightSources[_lightIndex].quadraticParameter = quadratic;
		_lightSources[_lightIndex].type = POINT;
	}
	else {
		// If the limit is reached, does nothing and lets the user know why
		std::cout << "Maximum amount of light sources has already been reached" << std::endl;
	}
}
void Shader::makeLightSource(glm::vec3 color, glm::vec3 direction, float constant, float  linear, float  quadratic, float cutOff, float outerCutOff) {
	// Checks if other light sources can be added
	if (_lightCount <= MAX_N_LIGHTS) {
		// Marks shader as a light source, sets the parameters for a new light source as asked and updates the vector and counter
		makeLightSource(color, constant, linear, quadratic);
		_lightSources[_lightIndex].direction = glm::normalize(direction);
		_lightSources[_lightIndex].cutOff = cutOff;
		_lightSources[_lightIndex].outerCutOff = outerCutOff;
		_lightSources[_lightIndex].type = SPOTLIGHT;
	}
	else {
		// If the limit is reached, does nothing and lets the user know why
		std::cout << "Maximum amount of light sources has already been reached" << std::endl;
	}
}

void Shader::Use(Material material, glm::vec3 viewPos, glm::mat4 projection, glm::mat4 view, glm::mat4 model) {

	// Sets which shader program will be used
	glUseProgram(_shaderProgram[reflectionModel]);

	// For phong shading, passes the necessary variables
	// To do: implement flat shading
	// To do: might need a switch statement to pass different uniform variables on different shading programs (mainly because of flat shading)
	GLuint transformLoc;

	// Pass material information
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "material.colorRGB");
	glUniform3f(transformLoc, material.colorRGB.x, material.colorRGB.y, material.colorRGB.z);
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "material.ambient");
	glUniform3f(transformLoc, material.ambient.x, material.ambient.y, material.ambient.z);
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "material.diffuse");
	glUniform3f(transformLoc, material.diffuse.x, material.diffuse.y, material.diffuse.z);
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "material.specular");
	glUniform3f(transformLoc, material.specular.x, material.specular.y, material.specular.z);
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "material.shininess");
	glUniform1f(transformLoc, material.shininess);
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "material.transparency");
	glUniform1f(transformLoc, material.transparency);

	// Pass lights information
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "nLights");
	glUniform1i(transformLoc, _lightCount);
	for (int i = 0; i < _lightCount; i++) {
		std::string number = std::to_string(i);
		std::string aux;
		aux = "lightSources[" + number + "].position";
		//std::cout << "passing variable as: " << aux.c_str() << std::endl;
		transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], aux.c_str());
		glUniform3f(transformLoc, _lightSources[i].position.x, _lightSources[i].position.y, _lightSources[i].position.z);
		aux = "lightSources[" + number + "].direction";
		//std::cout << "passing variable as: " << aux.c_str() << std::endl;
		transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], aux.c_str());
		glUniform3f(transformLoc, _lightSources[i].direction.x, _lightSources[i].direction.y, _lightSources[i].direction.z);
		aux = "lightSources[" + number + "].colorRGB";
		//std::cout << "passing variable as: " << aux.c_str() << std::endl;
		transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], aux.c_str());
		glUniform3f(transformLoc, _lightSources[i].colorRGB.x, _lightSources[i].colorRGB.y, _lightSources[i].colorRGB.z);
		aux = "lightSources[" + number + "].cutOff";
		//std::cout << "passing variable as: " << aux.c_str() << std::endl;
		transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], aux.c_str());
		glUniform1f(transformLoc, _lightSources[i].cutOff);
		aux = "lightSources[" + number + "].outerCutOff";
		//std::cout << "passing variable as: " << aux.c_str() << std::endl;
		transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], aux.c_str());
		glUniform1f(transformLoc, _lightSources[i].outerCutOff);
		aux = "lightSources[" + number + "].constantParameter";
		//std::cout << "passing variable as: " << aux.c_str() << std::endl;
		transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], aux.c_str());
		glUniform1f(transformLoc, _lightSources[i].constantParameter);
		aux = "lightSources[" + number + "].linearParameter";
		//std::cout << "passing variable as: " << aux.c_str() << std::endl;
		transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], aux.c_str());
		glUniform1f(transformLoc, _lightSources[i].linearParameter);
		aux = "lightSources[" + number + "].quadraticParameter";
		//std::cout << "passing variable as: " << aux.c_str() << std::endl;
		transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], aux.c_str());
		glUniform1f(transformLoc, _lightSources[i].quadraticParameter);
		aux = "lightSources[" + number + "].type";
		//std::cout << "passing variable as: " << aux.c_str() << std::endl;
		transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], aux.c_str());
		glUniform1i(transformLoc, _lightSources[i].type);
	}

	// Pass other light parameters
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "viewPos");
	glUniform3f(transformLoc, viewPos.x, viewPos.y, viewPos.z);
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "lightAmbient");
	glUniform3f(transformLoc, 0.75f, 0.75f, 0.75f);
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "lightDiffuse");
	glUniform3f(transformLoc, 0.75f, 0.75f, 0.75f);
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "lightSpecular");
	glUniform3f(transformLoc, 0.5f, 0.5f, 0.5f);

	// Pass transform matrices information
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "view");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(view));
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "projection");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Shader::Update(glm::vec3 position) {
	// Whenever a light is moved, it's position has to be updated
	if (_type == LIGHT) {
		_lightSources[_lightIndex].position = position;
		std::cout << "(" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
	}
}

Shader::~Shader(){
	// Nothing to destroy
}