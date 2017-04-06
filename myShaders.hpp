#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

GLuint CreateSimpleVShader(GLint* success);

GLuint CreateBlueFShader(GLint* success);

GLuint CreateRedFShader(GLint* success);

GLuint CreateSingleColorFShader(float R_value, float G_value, float B_value, float A_value, GLint* success);

GLuint LinkShaderProgram(GLuint vertexShader, GLuint fragmentShader, GLint* success);