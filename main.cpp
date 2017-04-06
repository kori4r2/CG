#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "myShaders.hpp"
#include <iostream>

void key_callback(GLFWwindow*, int, int, int, int);

GLFWwindow *initWindow(int OpenGLverMajor, int OpenGLverMinor, int width, int height, const char *title);

int main() {

	GLFWwindow *window = initWindow(3, 3, 800, 600, "janela");
	if (!window)
		return -1;

//-------------------------------------------------------------------------------------------------------

	GLint success;

	// Create and compile vertex shader from code
	GLuint vertexShader = CreateSimpleVShader(&success);
	if (!success)
		return -1;

	// Create and compile blue fragment shader from code
	GLuint blueFShader = CreateBlueFShader(&success);
	if (!success)
		return -1;

	// Create and compile red fragment shader from code
	GLuint redFShader = CreateSingleColorFShader(1.0f, 0.0f, 0.0f, 1.0f, &success);
	if (!success)
		return -1;

	// Creates shader program for blue color, attaches shaders and links the program
	GLuint blueShaderProgram = LinkShaderProgram(vertexShader, blueFShader, &success);
	if (!success)
		return -1;

	// Creates shader program for red color
	GLuint redShaderProgram = LinkShaderProgram(vertexShader, redFShader, &success);
	if (!success)
		return -1;

	// The shaders are no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(blueFShader);
	glDeleteShader(redFShader);

//---------------------------------------------------------------------------------------------------------

	// The vertices of the object
	GLfloat vertices[] = {
		1.0f, 0.5f, 0.0f,
		1.0f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		-1.0f, -0.5f, 0.0f,
		-1.0f, 0.5f, 1.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 4
	};

//---------------------------------------------------------------------------------------------------------

	//GLuint EBO;
	//glGenBuffers(1, &EBO);

	// This is an array of Vertex Buffer objects
	GLuint VBOs[2];
	glGenBuffers(2, VBOs);

	// Create and bind the array of Vertex Array Objects
	GLuint VAOs[2];
	glGenVertexArrays(2, VAOs);

	// The first array positions will get the pixels of the red triangle
	glBindVertexArray(VAOs[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// This is not used here
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// The first buffer gets the first 3 vertices saved in the vertices array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// The second array position will get the pixels of the blue triangle
	glBindVertexArray(VAOs[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// The second buffer skips the first two pixels saved in the vertices array (tightly packed) and gets the last 3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(6 * (sizeof(GLfloat)) ) );
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

//---------------------------------------------------------------------------------------------------------

	// Game loop
	while (!glfwWindowShouldClose(window)) {
		// Checks if events were triggered
		glfwPollEvents();
		// Set color to clear buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clear color buffer
		glClear(GL_COLOR_BUFFER_BIT);
		// Draw the red triangle
		glUseProgram(redShaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// Draw the blue triangle
		glUseProgram(blueShaderProgram);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// Unbinds the vertex array
		glBindVertexArray(0);
		// Swaps the back buffer to front buffer, displaying in the output
		glfwSwapBuffers(window);
	}

//-----------------------------------------------------------------------------------------------------

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	// EBO was not used
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

GLFWwindow *initWindow(int OpenGLverMajor, int OpenGLverMinor, int width, int height, const char *title) {

	GLFWwindow *window;

	glfwInit();
	// Specify opengl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenGLverMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenGLverMinor);
	// Specify profile to be used
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Set window paraters
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a window
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	// Make it the current context
	glfwMakeContextCurrent(window);
	// Set the callback function
	glfwSetKeyCallback(window, key_callback);

	// Initialize glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		glfwTerminate();
		window = nullptr;
		return window;
	}

	// Set viewport dimensions
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
	glViewport(0, 0, bufferWidth, bufferHeight);

	return window;
}

// Define the callback for pressed keys
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	// ESC closes the window
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}