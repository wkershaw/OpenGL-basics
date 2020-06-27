#include "Renderer.h"
Renderer::Renderer()
{
    if (!glfwInit()) throw "glfw initialisation error!\n";
    window = glfwCreateWindow(resolution.x, resolution.y, "OpenGL basics", NULL, NULL);
    if (!window) { glfwTerminate(); throw "glfw window creation error!\n"; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //Sync the buffer swapping with 1 screen update (vsync)
    if (glewInit() != GLEW_OK) std::cout << "Error!" << std::endl;

    glClearColor(clearColour.r, clearColour.g, clearColour.b, clearColour.a);
}

Renderer::~Renderer()
{
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

bool Renderer::WindowShouldClose() {
    return glfwWindowShouldClose(window);
}

void Renderer::EnableBlending()
{
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //Enable alpha blending
    GLCALL(glEnable(GL_BLEND))
}

void Renderer::EnableDepth()
{
    GLCALL(glEnable(GL_DEPTH_TEST));
}

void Renderer::Clear() const
{
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    va.Bind();
    ib.Bind();
    shader.Bind();
    GLCALL(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); //Draw the vertices using the index buffer

}

void Renderer::Draw(Object* object, Shader* shader)
{

    glm::mat4 model = glm::translate(glm::mat4(1.0f), object->GetTranslation());
    model = glm::rotate(model, object->GetRotation().x, glm::vec3(1, 0, 0));
    model = glm::rotate(model, object->GetRotation().y, glm::vec3(0, 1, 0));
    model = glm::rotate(model, object->GetRotation().z, glm::vec3(0, 0, 1));
    model = glm::scale(model, object->GetScale());

    float* lightPositions = new float[lights.size() * 3];
    float* lightColours = new float[lights.size() * 3];

    for (unsigned int i = 0; i < lights.size(); i++) {
        lightPositions[i] = lights[i]->GetPosition().x;
        lightPositions[i + 1] = lights[i]->GetPosition().y;
        lightPositions[i + 2] = lights[i]->GetPosition().z;

        lightColours[i] = lights[i]->GetColour().r;
        lightColours[i + 1] = lights[i]->GetColour().g;
        lightColours[i + 2] = lights[i]->GetColour().b;
    }

    object->Bind(shader);
    shader->SetUniformMat4f("u_proj", proj);
    shader->SetUniformMat4f("u_view", view);
    shader->SetUniformMat4f("u_model", model);
    shader->SetUniform3f("u_cameraPosition", cameraPosition);
    shader->SetUniform3fv("u_lightPositions", lightPositions, lights.size());
    shader->SetUniform3fv("u_lightColours", lightColours, lights.size());

    

    if (object->GetDrawMode() == Object::DrawMode::TRIANGLES) {
        GLCALL(glDrawElements(GL_TRIANGLES, object->GetIndexCount(), GL_UNSIGNED_INT, nullptr)); //Draw the vertices using the index buffer
    }
    else {
        GLCALL(glDrawElements(GL_TRIANGLE_STRIP, object->GetIndexCount(), GL_UNSIGNED_INT, nullptr)); //Draw the vertices using the index buffer
    }
    
    shader->Unbind();
    object->Unbind();

}

void Renderer::StartImGui() {
    //ImGui instantiantion
    ImGui::CreateContext();
    ImGui::StyleColorsDark(); //Dark mode B-)
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void Renderer::NewImGuiFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Renderer::DrawFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
}
