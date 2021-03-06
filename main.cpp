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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// Creates a window using the desired parameters
GLFWwindow *initWindow(int OpenGLverMajor, int OpenGLverMinor, int width, int height, const char *title);
// Global variables
GLuint screenWidth, screenHeight;
bool clickFlag = false, jumpFlag = false, keys[1024];
double clickX, clickY, xPos, yPos, yScroll = 0.0f;

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
	Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), keys, window, &xPos, &yPos, &yScroll);
	// Sets camera values
	camera->setSpeedValue(100.0f);
	//camera->enableGravity(); // Allows the camera to jump

//-------------------------------------------------------------------------------------------------------

	GLint success;

	// Create and compile vertex shader from code
	GLuint vertexShader = CreateSimpleVShader(&success);
	if (!success)
		return -1;

	// Creates white metal material for polyhedrons
	Material metalMaterial(METAL, glm::vec3(1.0f, 1.0f, 1.0f));
	// Creates white glass material for polyhedrons
	Material glassMaterial(GLASS, glm::vec3(1.0f, 1.0f, 1.0f));
	// Creates white plastic material for polyhedrons
	Material plasticMaterial(PLASTIC, glm::vec3(1.0f, 1.0f, 1.0f));

	// Creates brown default material for the ground (plane)
	Material brownMaterial(DEFAULT, glm::vec3(0.9f, 0.3f, 0.015f));

	// Create and compile semi transparent red fragment shader from code
	GLuint redFShader = CreateSingleColorFShader(0.9f, 0.1f, 0.1f, 0.5f, &success);
	if (!success)
		return -1;
	// Create and compile semi transparent yellow fragment shader from code
	GLuint yellowFShader = CreateSingleColorFShader(0.9f, 0.9f, 0.1f, 0.5f, &success);
	if (!success)
		return -1;

	// Creates shader program for red color, attaches shaders and links the program
	GLuint redShaderProgram = LinkShaderProgram(vertexShader, redFShader, &success);
	if (!success)
		return -1;
	// Creates shader program for green color, attaches shaders and links the program
	GLuint yellowShaderProgram = LinkShaderProgram(vertexShader, yellowFShader, &success);
	if (!success)
		return -1;


	// The shaders are no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(redFShader);
	glDeleteShader(yellowFShader);

	// Creates a red semi transparent square to be positioned in the middle of the screen
	Polygon *square = new Polygon((screenWidth / 2.0f), (screenHeight / 2.0f), 15, 4, camera, window);
	square->setShaderProgram(&redShaderProgram);

	Polyhedron *objects[20];
	int nObjects = 0;

	// "Sun"
	objects[nObjects++] = new Sphere(0.0f, 1000.0f, 0.0f, 100.0f, camera, window);
	// 3 light sources
	objects[nObjects++] = new Sphere(0.0f, 200.0f, -400.0f, 20.0f, camera, window);
	objects[nObjects++] = new Sphere(282.843154416f, 200.0f, 89.898714f, 20.0f, camera, window);
	objects[nObjects++] = new Sphere(-282.843154416f, 200.0f, 89.898714f, 20.0f, camera, window);
	// Polihedrons that can be thrown
	objects[nObjects++] = new Sphere(0.0f, 60.0f, -200.0f, 20.0f, camera, window);
	objects[nObjects++] = new Cube(200.0f, 60.0f, -200.0f, 20.0f, camera, window);
	objects[nObjects++] = new Octahedron(-200.0f, 60.0f, -200.0f, 20.0f, camera, window);

	// Set light sources
	// Sets a directional light
	objects[0]->makeLightSource(glm::vec3(1.0f, 1.0f, 0.9f), glm::vec3(0.0f, -1.0f, 0.0f));
	// Sets a point light
	objects[1]->makeLightSource(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0035f, 0.0001f);
	// Sets a downward pointing green spotlight
	objects[2]->makeLightSource(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 10.f, 0.0007f, 0.00002f, glm::cos(glm::radians(40.0f)), glm::cos(glm::radians(45.0f)));
	// Sets a downward pointing purple spotlight
	objects[3]->makeLightSource(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), 10.f, 0.0007f, 0.00002f, glm::cos(glm::radians(40.0f)), glm::cos(glm::radians(45.0f)));

	// Sets references to movable objects to make code more understandable
	Sphere *sphere = (Sphere*)objects[4];
	Cube *cube = (Cube*)objects[5];
	Octahedron *octahedron = (Octahedron*)objects[6];

	// Activates gravity on objects
	sphere->enableGravity();
	cube->enableGravity();
	octahedron->enableGravity();

	// Sets materials to objects
	octahedron->setMaterial(glassMaterial);
	cube->setMaterial(plasticMaterial);
	sphere->setMaterial(metalMaterial);

	// Creates a brown plane
	Plane *plane = new Plane(0.0f, -0.1f, 0.0f, 5000.0f, camera, window);
	plane->setMaterial(brownMaterial);

//---------------------------------------------------------------------------------------------------------
	bool jump = true;
	// Game loop
	while (!glfwWindowShouldClose(window)) {

		// Checks if events were triggered
		glfwPollEvents();

		//Checks mouseclick
		if (clickFlag) {
			// Changes reflection model
			Shader::reflectionModel = (Shader::reflectionModel == PHONG) ? GOURAUD : PHONG;
			clickFlag = false;
			square->setShaderProgram((Shader::reflectionModel == PHONG)? &redShaderProgram: &yellowShaderProgram);
		}
		
		// Checks spacebar press
		if (jumpFlag) {
			// Jumps with the camera
			//camera->jump(100);
			// Activates speed in the polyhedrons
			if(jump) {
				// The rotation axis of the polyhedrons is completely random (-1.0 ~ 1.0, -1.0 ~ 1.0, -1.0 ~ 1.0)
				cube->setAngularSpeed(glm::vec3(((std::rand() % 201) * 0.01f) - 1.0f, ((std::rand() % 201) * 0.01f) - 1.0f, ((std::rand() % 201) * 0.01f) - 1.0f), 90.0f);
				// Each polyhedron gets throw up in the y axis, but in a random direction on the xz plane (the speed direction is (-0.5 ~ 0.5, 1.0, -0.5 ~ 0.5))
				cube->setSpeed(glm::vec3(((std::rand() % 101) * 0.01f) -0.5f, 1.0f, ((std::rand() % 101) * 0.01f) - 0.5f), 200.0f);
				// The speed and angular speed of the octahedron and the sphere are set just like the cube's
				octahedron->setAngularSpeed(glm::vec3(((std::rand() % 201) * 0.01f) - 1.0f, ((std::rand() % 201) * 0.01f) - 1.0f, ((std::rand() % 201) * 0.01f) - 1.0f), 90.0f);
				octahedron->setSpeed(glm::vec3(((std::rand() % 101) * 0.01f) - 0.5f, 1.0f, ((std::rand() % 101) * 0.01f) - 0.5f), 200.0f);
				sphere->setAngularSpeed(glm::vec3(((std::rand() % 201) * 0.01f) - 1.0f, ((std::rand() % 201) * 0.01f) - 1.0f, ((std::rand() % 201) * 0.01f) - 1.0f), 90.0f);
				sphere->setSpeed(glm::vec3(((std::rand() % 101) * 0.01f) - 0.5f, 1.0f, ((std::rand() % 101) * 0.01f) - 0.5f), 200.0f);
				// Changes the bool variable, to make the next spacebar press stop the objects, instead of throw them
				jump = false;
			} else {	
				// Stops all object movements
				cube->setAngularSpeed(0.0f);
				cube->setSpeed(0.0f);
				octahedron->setAngularSpeed(0.0f);
				octahedron->setSpeed(0.0f);
				sphere->setAngularSpeed(0.0f);
				sphere->setSpeed(0.0f);
				// Change the bool variable, to make the the next spacebar press throw the objects, instead of stop them
				jump = true;
			}
			// Resets the jumpFlag to indicate that the spacebar press has been processed
			jumpFlag = false;
		}

		// Updates camera position
		camera->Update();
		// Set color to clear buffer
		glClearColor(0.6f, 0.6f, 0.9f, 1.0f);
		// Clear color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Updates and draws the objects

		plane->Update();
		plane->Draw();

		for (int i = 0; i < nObjects; i++) {
			objects[i]->Update();
			objects[i]->Draw();
		}

		// GUI (2D objects) update/draw functions must be called after 3D objects
		square->Update();
		square->Draw();

		// Swaps the back buffer to front buffer, displaying in the output
		glfwSwapBuffers(window);
	}

//-----------------------------------------------------------------------------------------------------
	// Ends execution
	for (int i = 0; i < nObjects; i++)
		delete(objects[i]);
	delete(plane);
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
	glfwSetScrollCallback(window, scroll_callback);

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
		glfwGetCursorPos(window, &clickX, &clickY);
		std::cout << "pressed" << clickX << "\t" << clickY << std::endl;
		clickY = screenHeight - clickY;
		clickFlag = true;
	}
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
	// Updates variables with mouse position
	yPos = ypos;
	xPos = xpos;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	// Call camera class function that updates the zoom
	yScroll = yoffset;
}