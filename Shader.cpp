#include "Shader.hpp"


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

Material::Material(glm::vec3 color) 
	:Material(){
	_colorRGB = color;
}

Material::Material(Materials type, glm::vec3 color)
	: Material(){
	_colorRGB = color;
	// To do
	_ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	_shininess = 1.0f;
	_transparency = 1.0f;
}

Material& Material::operator=(const Material &original) {
	_colorRGB = original.colorRGB;
	_ambient = original.ambient;
	_diffuse = original.diffuse;
	_specular = original.specular;
	_shininess = original.shininess;
	_transparency = original.transparency;
	return *this;
}

Material::~Material() {
}

int Shader::_lightCount = 0;
std::vector<LightSource> Shader::_lightSources = std::vector<LightSource>(0);
ReflectionModel Shader::reflectionModel = PHONG;
bool Shader::_programsCompiled = false;
GLuint Shader::_shaderProgram[3] = {0, 0, 0};

GLuint Shader::linkProgram(const char *vShaderFilename, const char *fShaderFilename, GLint *success) {
	std::ifstream vShaderFile, fShaderFile;
	std::stringstream vShaderStream, fShaderStream;
	GLuint vertexShader, fragmentShader, shaderProgram;
	char infolog[1024];

	vShaderFile.open(vShaderFilename);
	fShaderFile.open(fShaderFilename);
	if (vShaderFile.bad() || fShaderFile.bad()) {
		if (!vShaderFile.bad())
			vShaderFile.close();
		else
			fShaderFile.close();
		std::cout << "Failed to load shader files" << std::endl;
		return 0;
	}
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();
	vShaderFile.close();
	fShaderFile.close();
	std::string auxString1 = vShaderStream.str();
	std::string auxString2 = fShaderStream.str();
	const char *vertexSource = auxString1.c_str();
	const char *fragmentSource = auxString2.c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, success);
	if (!(*success)) {
		glGetShaderInfoLog(vertexShader, 1024, nullptr, infolog);
		std::cout << "Failed to compile vertex shader" << infolog << std::endl;
		return 0;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, success);
	if (!(*success)) {
		glGetShaderInfoLog(fragmentShader, 1024, nullptr, infolog);
		std::cout << "Failed to compile fragment shader" << infolog << std::endl;
		return 0;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, success);
	if (!(*success)) {
		glGetProgramInfoLog(shaderProgram, 1024, nullptr, infolog);
		std::cout << "Failed to link shader program" << infolog << std::endl;
		return 0;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}


Shader::Shader() {
	if (!_programsCompiled) {
		GLint success = 1;
		_shaderProgram[PHONG] = linkProgram("light_materials_phong.vs", "light_materials_phong.fs", &success);
		if (!success) {
			system("pause");
			exit(1);
		}
		//_shaderProgram[1] = linkProgram("light_materials_gourand.vs", "light_materials_gourand.fs", &success);
		//if (!success){
		//	system("pause");
		//	exit(1);
		//}
		//_shaderProgram[2] = linkProgram("light_materials_flat.vs", "light_materials_flat.fs", &success);
		//if (!success){
		//	system("pause");
		//	exit(1);
		//}
		_programsCompiled = true;
	}
	_type = MATERIAL;
	_lightIndex = -1;
}

void Shader::makeLightSource(LightType lightType, glm::vec3 position, glm::vec3 color, glm::vec3 direction) {
	_type = LIGHT;
	_lightIndex = _lightCount++;
	LightSource thisLight;
	thisLight.colorRGB = color;
	thisLight.direction = direction;
	thisLight.type = lightType;
	thisLight.position = position;
	_lightSources.push_back(thisLight);
}

void Shader::Use(Material material, glm::vec3 position, glm::mat4 projection, glm::mat4 view, glm::mat4 model) {
	if (_type == LIGHT) {
		_lightSources[_lightIndex].position = position;
	}

	glUseProgram(_shaderProgram[reflectionModel]);

	GLuint transformLoc;
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

	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "view");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(view));
	transformLoc = glGetUniformLocation(_shaderProgram[reflectionModel], "projection");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

Shader::~Shader(){
}