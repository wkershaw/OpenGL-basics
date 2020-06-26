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

    bool WindowShouldClose();

    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Draw(Object* object, Shader* shader);

    void StartImGui();
    void NewImGuiFrame();

    void DrawFrame();

    void SetProjectionMatrix(glm::mat4 proj) { this->proj = proj; };
    void SetViewMatrix(glm::mat4 view) { this->view = view; };
    void SetCameraPosition(glm::vec3 cameraPosition) { this->cameraPosition = cameraPosition; };
    void SetClearColour(glm::vec4 colour) { clearColour = colour; };

private:
    GLFWwindow* window;

    glm::vec3 resolution = glm::vec3(1000, 800, 1000);
    glm::vec4 clearColour = glm::vec4(0.1f, 0.1f, 0.1f, 0.1f);

    glm::mat4 proj;
    glm::mat4 view;
    glm::vec3 cameraPosition;

};