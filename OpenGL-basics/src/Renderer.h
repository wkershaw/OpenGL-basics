#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Object.h"
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
    glm::vec3 cameraPosition;

private:

};