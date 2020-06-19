#include "Renderer.h"


void GLClearError() { //Clear all OpenGL error flags
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGL error " << function <<
            " in " << file << " on line "
            << line << " : "
            << error << std::endl;
        return false;
    }
    return true;
}

Renderer::Renderer()
{
    /* Initialize the library */
    if (!glfwInit())
        throw "glfw initialisation error!\n";

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1000, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "glfw window creation error!\n";
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); //Sync the buffer swapping with 1 screen update (vsync)

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

    std::cout << "OpenGL loaded: " << glGetString(GL_VERSION) << std::endl;

    proj = glm::perspective(45.0f, 1000 / 800.0f, 1.0f, 1000.0f);
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0));


    //ImGui instantiantion
    ImGui::CreateContext();
    ImGui::StyleColorsDark(); //Dark mode B-)
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

}

Renderer::~Renderer()
{
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
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
    glm::mat4 MVP = proj * view * model; //Move image 1 to translated spot
    object->Bind(shader);
    shader->SetUniformMat4f("u_MVP", MVP);

    GLCALL(glDrawElements(GL_TRIANGLES, object->GetIndexCount(), GL_UNSIGNED_INT, nullptr)); //Draw the vertices using the index buffer
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
