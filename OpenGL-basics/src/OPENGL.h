#pragma once

//Macros for debugging
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

