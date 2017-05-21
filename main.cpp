#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "myShaders.hpp"
#include "Polygon.hpp"
#include "my3dObjects.hpp"
#include "Camera.hpp"
#include <iostream>

/*
// Debugging
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
int nada = _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif
*/

// Callback functions
void key_callback(GLFWwindow*, int, int, int, int);
void mouse_button_callback(GLFWwindow*, int, int, int);
void mouse_callback(GLFWwindow*, double, double);
// Creates a window using the desired parameters
GLFWwindow *initWindow(int OpenGLverMajor, int OpenGLverMinor, int width, int height, const char *title);
// Global variables
GLuint screenWidth, screenHeight;
bool movFlag = false, jumpFlag = false, keys[1024];
double movX, movY, xPos, yPos;

int main() {

	// Creates the window
	GLFWwindow *window = initWindow(3, 3, 800, 600, "janela");
	if (!window)
		return -1;

	// Enables depth test for z-buffer
	glEnable(GL_DEPTH_TEST);
	// Enable color blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Creates the camera
	Camera *camera = new Camera(keys, window, &xPos, &yPos);
	// Sets camera values
	camera->setSpeedValue(70.0f);
	camera->enableGravity();

//-------------------------------------------------------------------------------------------------------

	GLint success;

	// Create and compile vertex shader from code
	GLuint vertexShader = CreateSimpleVShader(&success);
	if (!success)
		return -1;

	// Create and compile blue fragment shader from code
	GLuint blueFShader = CreateSingleColorFShader(0.1f, 0.1f, 0.9f, 1.0f, &success);
	if (!success)
		return -1;

	// Create and compile semi transparent red fragment shader from code
	GLuint redFShader = CreateSingleColorFShader(0.9f, 0.1f, 0.1f, 0.5f, &success);
	if (!success)
		return -1;

	// Create and compiler brown fragment shader from code
	GLuint brownFShader = CreateSingleColorFShader(0.3f, 0.1f, 0.05f, 1.0f, &success);
	if (!success)
		return -1;

	// Creates shader program for blue color, attaches shaders and links the program
	GLuint blueShaderProgram = LinkShaderProgram(vertexShader, blueFShader, &success);
	if (!success)
		return -1;

	// Creates shader program for red color, attaches shaders and links the program
	GLuint redShaderProgram = LinkShaderProgram(vertexShader, redFShader, &success);
	if (!success)
		return -1;

	// Creates shader program for brown color, attaches shaders and links the program
	GLuint brownShaderProgram = LinkShaderProgram(vertexShader, brownFShader, &success);
	if (!success)
		return -1;


	// The shaders are no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(blueFShader);
	glDeleteShader(redFShader);
	glDeleteShader(brownFShader);

	// Creates a red square
	Polygon *square = new Polygon((screenWidth / 2), (screenHeight / 2), 25, 4, camera, window);
	square->setShaderProgram(&redShaderProgram);

	// Creates a blue rotating cube
	Cube *cube = new Cube(100.0f, 30.0f, -400.0f, 30.0f, camera, window);
	cube->setAngularSpeed(glm::vec3(0.3f, 0.1f, -0.1f), 72.0f);
	// Throws it up and activates gravity
	cube->setSpeed(glm::vec3(0.0f, 1.0f, 0.0f), 400);
	cube->enableGravity();
	cube->setShaderProgram(&blueShaderProgram);

	// Creates a blue rotating tetrahedron
	Tetrahedron *tetrahedron = new Tetrahedron(0.0f, 200.0f, -400.0f, 30.0f, camera, window);
	tetrahedron->setAngularSpeed(glm::vec3(-0.3f, 0.2f, 0.4f), 72.0f);
	tetrahedron->enableGravity();
	tetrahedron->setShaderProgram(&blueShaderProgram);

	// Creates a blue rotating sphere
	Sphere *sphere = new Sphere(-100.0f, 150.0f, -400.0f, 15.0f, camera, window);
	sphere->setAngularSpeed(glm::vec3(0.3f, 0.1f, -0.1f), 72.0f);
	sphere->enableGravity();
	sphere->setShaderProgram(&blueShaderProgram);

	// Creates a brown plane
	Plane *plane = new Plane(0.0f, -0.1f, 0.0f, 5000.0f, camera, window);
	plane->setShaderProgram(&brownShaderProgram);

//---------------------------------------------------------------------------------------------------------
	// Game loop
	while (!glfwWindowShouldClose(window)) {

		// Checks if events were triggered
		glfwPollEvents();

		/*
		//Checks mouseclick
		if (movFlag) {
			// Moves square to position clicked
			square->setSpeed(glm::vec3((movX - square->x()), (movY - square->y()), 0.0f), 200.0f);
			movFlag = false;
		}
		*/

		// Checks spacebar press
		if (jumpFlag) {
			// Jumps with the camera
			camera->jump(100);
			jumpFlag = false;
		}

		// Updates camera position
		camera->Update();
		// Set color to clear buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// Clear color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Updates and draws the objects

		plane->Update();
		plane->Draw();

		cube->Update();
		cube->Draw();

		tetrahedron->Update();
		tetrahedron->Draw();

		sphere->Update();
		sphere->Draw();

		// GUI (2D objects) update/draw functions must be called after 3D objects
		square->Update();
		square->Draw();

		// Swaps the back buffer to front buffer, displaying in the output
		glfwSwapBuffers(window);
	}

//-----------------------------------------------------------------------------------------------------
	// Ends execution
	delete(plane);
	delete(sphere);
	delete(tetrahedron);
	delete(cube);
	delete(square);
	delete(camera);
	glfwTerminate();

	return 0;
}

GLFWwindow *initWindow(int OpenGLverMajor, int OpenGLverMinor, int width, int height, const char *title) {

	GLFWwindow *window;
	// Saves window dimensions
	screenWidth = width;
	screenHeight = height;

	// Sets initial value for keys vector
	glfwInit();
	for (int i = 0; i < 1024; i++)
		keys[i] = false;

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

	// Hides mouse cursor, and make sure it stays inside the window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set the callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// Initialize glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		glfwTerminate();
		window = nullptr;
		return window;
	}

	// Set viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	return window;
}

// Define the callback for pressed keys
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	// ESC closes the window
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	// Space bar sets a jump flag
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		jumpFlag = true;
	// Any key press/release alters the state inside the keys vector
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	// Saves the position clicked and sets a corresponding flag
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwGetCursorPos(window, &movX, &movY);
		std::cout << "pressed" << movX << "\t" << movY << std::endl;
		movY = screenHeight - movY;
		movFlag = true;
	}
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
	// Updates variables with mouse position
	yPos = ypos;
	xPos = xpos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	// Call camera class function that updates the zoom
	camera->ProcessMouseScroll(yoffset);
}