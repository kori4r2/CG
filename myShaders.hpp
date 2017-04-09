#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

// Creates simples vertex shader whith transform variable to apply movement to the object
GLuint CreateSimpleVShader(GLint* success);

// Simple blue fragment shader
GLuint CreateBlueFShader(GLint* success);

// Simple red fragment shader
GLuint CreateRedFShader(GLint* success);

// Simple fragment shader with the desired RGBA color
GLuint CreateSingleColorFShader(float R_value, float G_value, float B_value, float A_value, GLint* success);

// Links the vertex and fragment shaders to return a shader program
GLuint LinkShaderProgram(GLuint vertexShader, GLuint fragmentShader, GLint* success);