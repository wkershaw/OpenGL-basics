#pragma once
//Macros for debugging
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Object.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
public:

    Renderer();
    ~Renderer();

    void EnableBlending();
    void EnableDepth();

    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Draw(Object* object, Shader* shader);

    void NewImGuiFrame();

    void DrawFrame();

    GLFWwindow* window;

    glm::mat4 proj;
    glm::mat4 view;

private:

};