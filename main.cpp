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

void mouse_button_callback(GLFWwindow*, int, int, int);

GLFWwindow *initWindow(int OpenGLverMajor, int OpenGLverMinor, int width, int height, const char *title);
GLuint screenWidth, screenHeight;

bool movFlag = false;
double movX, movY;

int main() {

	int width, height;

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

	// Create and compile red fragment shader from code
	GLuint greenFShader = CreateSingleColorFShader(0.0f, 1.0f, 0.0f, 1.0f, &success);
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

	// Creates shader program for green color
	GLuint greenShaderProgram = LinkShaderProgram(vertexShader, greenFShader, &success);
	if (!success)
		return -1;

	// The shaders are no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(blueFShader);
	glDeleteShader(redFShader);
	glDeleteShader(greenFShader);

	// Create polygons to be drawn on screen (best way would be to save inside a Polygon[] variable)
	Polygon *triangle = new Polygon(200, 150, 50, 3, window);
	// Associates the object with a shader program
	triangle->setShaderProgram(&blueShaderProgram);
	Polygon *square = new Polygon(600, 450, 100, 4, window);
	square->setShaderProgram(&redShaderProgram);
	Polygon *pentagon = new Polygon(350, 350, 30, 5, window);
	pentagon->setShaderProgram(&greenShaderProgram);

//---------------------------------------------------------------------------------------------------------
	// Game loop
	while (!glfwWindowShouldClose(window)) {

		//Checks mouseclick
		if (movFlag) {
			square->setSpeed(glm::vec3((movX - square->x()), (movY - square->y()), 0.0f), 100.0f);
			triangle->setSpeed(glm::vec3((movX - triangle->x()), (movY - triangle->y()), 0.0f), 200.0f);
			pentagon->setSpeed(glm::vec3((movX - pentagon->x()), (movY - pentagon->y()), 0.0f), 400.0f);
			movFlag = false;
		}


		// Checks if events were triggered
		glfwPollEvents();
		// Set color to clear buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clear color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Calls update and draw from the triangle object
		triangle->Update();
		triangle->Draw();

		// Calls update and draw from the square object
		square->Update();
		square->Draw();

		//// Calls update and draw from the pentagon object
		pentagon->Update();
		pentagon->Draw();

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
	screenWidth = width;
	screenHeight = height;

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
	glfwSetMouseButtonCallback(window, mouse_button_callback);

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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &movX, &movY);
		std::cout << "pressed" << movX << "\t" << movY << std::endl;
		movY = screenHeight - movY;
		movFlag = true;
	}
}