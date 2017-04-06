#include "myShaders.hpp"

// Vertex shader code
const GLchar* simpleVShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"void main() {\n"
"	gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

// Fragment shader code
// This fragment shader paints the pixels blue
const GLchar* blueFShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.0, 0.0, 1.0, 1.0);\n"
"}\n\0";

// This fragment shader paints the pixels red
const GLchar* redFShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}\n\0";

GLuint CreateSimpleVShader(GLint *success) {

	// Create and compile vertex shader from code
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &simpleVShaderSource, nullptr);
	glCompileShader(vertexShader);

	// Check if compilation was successful
	GLchar infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, success);
	if (!(*success)) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infolog);
		std::cerr << "ERROR: Vertex shader compilation error" << std::endl << infolog << std::endl;
		return 0;
	}

	return vertexShader;
}

GLuint CreateBlueFShader(GLint *success) {

	// Create and compile blue fragment shader from code
	GLuint blueFShader;
	GLchar infolog[512];
	blueFShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(blueFShader, 1, &blueFShaderSource, nullptr);
	glCompileShader(blueFShader);

	// Check if compilation was successful
	glGetShaderiv(blueFShader, GL_COMPILE_STATUS, success);
	if (!(*success)) {
		glGetShaderInfoLog(blueFShader, 512, nullptr, infolog);
		std::cerr << "ERROR: Blue color fragment shader compilation error" << std::endl << infolog << std::endl;
		return 0;
	}

	return blueFShader;
}

GLuint CreateRedFShader(GLint *success) {

	// Create and compile red fragment shader from code
	GLuint redFShader;
	GLchar infolog[512];
	redFShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(redFShader, 1, &redFShaderSource, nullptr);
	glCompileShader(redFShader);

	// Check if compilation was successful
	glGetShaderiv(redFShader, GL_COMPILE_STATUS, success);
	if (!(*success)) {
		glGetShaderInfoLog(redFShader, 512, nullptr, infolog);
		std::cerr << "ERROR: Red color fragment shader compilation error" << std::endl << infolog << std::endl;
		return 0;
	}

	return redFShader;
}

GLuint CreateSingleColorFShader(float Rvalue, float Gvalue, float Bvalue, float Avalue, GLint *success) {

	std::string mycode = "#version 330 core\nout vec4 color;\nvoid main()\n{\ncolor = vec4("
		+ std::to_string(Rvalue) + ", " + std::to_string(Gvalue) + ", " + std::to_string(Bvalue) + ", " + std::to_string(Avalue) +
		");\n}\n\0";
	const GLchar* colorFShaderSource = mycode.c_str();

	// Create and compile fragment shader from code
	GLuint colorFShader;
	GLchar infolog[512];
	colorFShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(colorFShader, 1, &colorFShaderSource, nullptr);
	glCompileShader(colorFShader);

	// Check if compilation was successful
	glGetShaderiv(colorFShader, GL_COMPILE_STATUS, success);
	if (!(*success)) {
		glGetShaderInfoLog(colorFShader, 512, nullptr, infolog);
		std::cerr << "ERROR: Color fragment shader compilation error" << std::endl << infolog << std::endl;
		return 0;
	}

	return colorFShader;
}

GLuint LinkShaderProgram(GLuint vertexShader, GLuint fragmentShader, GLint *success) {

	// Creates shader program for red color
	GLuint shaderProgram;
	GLchar infolog[512];
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check if linking was successful
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, success);
	if (!(*success)) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infolog);
		std::cout << "ERROR: Red shader program linking error" << std::endl << infolog << std::endl;
		return 0;
	}

	return shaderProgram;
}