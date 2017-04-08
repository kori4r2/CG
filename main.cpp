#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "myShaders.hpp"
#include "Polygon.hpp"
#include <iostream>

void key_callback(GLFWwindow*, int, int, int, int);

GLFWwindow *initWindow(int OpenGLverMajor, int OpenGLverMinor, int width, int height, const char *title);

int main() {

	// Creates the window
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

	// Create polygons to be drawn on screen (best way would be to save inside an array)
	Polygon *triangle = new Polygon(200, 150, 100, 3, window);
	Polygon *square = new Polygon(600, 450, 100, 4, window);

	// Set speed and behaviours
	// To do

//---------------------------------------------------------------------------------------------------------
	// Game loop
	while (!glfwWindowShouldClose(window)) {
		// Checks if events were triggered
		glfwPollEvents();
		// Set color to clear buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clear color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the blue triangle
		glUseProgram(blueShaderProgram);

		// triangle->Update();
		glm::mat4 moveTransform;
		GLfloat currentTime = glfwGetTime();
		moveTransform = glm::translate(moveTransform, glm::vec3( currentTime * 1.0f, 0.0f, 0.0f));
		//moveTransform = glm::rotate(moveTransform, currentTime * 50.0f, glm::vec3(0.0f, 0.0f, 0.1f));
		GLuint transformLoc2 = glGetUniformLocation(blueShaderProgram, "transform");
		glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, glm::value_ptr(moveTransform));

		triangle->Draw();

		// Draw the red triangle
		glUseProgram(redShaderProgram);

		// square->Update()
		glm::mat4 idMatrix;
		GLuint transformLoc = glGetUniformLocation(redShaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(idMatrix));

		square->Draw();

		// Swaps the back buffer to front buffer, displaying in the output
		glfwSwapBuffers(window);
	}

//-----------------------------------------------------------------------------------------------------
	delete(triangle);
	delete(square);

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