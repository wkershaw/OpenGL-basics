#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glfwSwapInterval(1); //Sync the buffer swapping with 1 screen update (vsync)

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

    std::cout << "OpenGL loaded: " <<glGetString(GL_VERSION) << std::endl;

    float positions[] = { //Vertex positions for a square
            -50, -50, 0.0f, 0.0f,
             50, -50, 1.0f, 0.0f,
             50,  50, 1.0f, 1.0f,
            -50,  50, 0.0f, 1.0f,
    };

    unsigned int indices[] = { //Inices to draw a square
        0,1,2,
        2,3,0
    };

    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //Enable alpha blending
    GLCALL(glEnable(GL_BLEND))

    glm::mat4 proj = glm::ortho(0.0f, 680.0f, 0.0f , 480.0f, -1.0f, 1.0f); //Create an orthographic matrix
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0));

    Texture* texture = new Texture("res/textures/colourSplash.png");
    Shader* shader = new Shader("res/shaders/basic.shader"); //Create a new shader    
    texture->Bind();
    shader->Bind();
    shader->SetUniform1i("u_texture", 0); //Bind the texture to slot 0

    VertexArray* va = new VertexArray(); //Make a new vertex array object to store the VB and its layout
    VertexBuffer* vb = new VertexBuffer(positions, 4 * 4 * sizeof(float)); //Make a buffer to store the vertex positions
    
    
    VertexBufferLayout* layout = new VertexBufferLayout(); //Define the layout for the buffer
    layout->Push<float>(2); //Each position is made up of 2 floats
    layout->Push<float>(2);
    
    
    va->AddBuffer(*vb, *layout); //Update the vertex array object with the position data
    IndexBuffer* ib = new IndexBuffer(indices, 6); //Define which order the vertices should be drawn in
    
    Renderer* renderer = new Renderer(); //Create a new render to make draw calls


    //ImGui instantiantion
    ImGui::CreateContext();
    ImGui::StyleColorsDark(); //Dark mode B-)
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    glm::vec3 translation1 = glm::vec3(100.0f, 100.0f, 0); //Translation for image 1
    glm::vec3 translation2 = glm::vec3(300.0f, 100.0f, 0); //Translation for image 2

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer->Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SliderFloat3("Translation1", &translation1.x, 0.0f, 680.0f); //Set the translation for image 1
        ImGui::SliderFloat3("Translation2", &translation2.x, 0.0f, 680.0f); //Set the translation for image 2
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); //Output framerate

        va->Bind();
        ib->Bind();
        texture->Bind();
        shader->Bind();


        
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation1);
        glm::mat4 MVP = proj * view * model; //Move image 1 to translated spot
        shader->SetUniformMat4f("u_MVP", MVP);
        renderer->Draw(*va, *ib, *shader); //Draw image 1
        

        model = glm::translate(glm::mat4(1.0f), translation2);
        MVP = proj * view * model;
        shader->SetUniformMat4f("u_MVP", MVP); //Move image 2 to translated spot
        renderer->Draw(*va, *ib, *shader); //Draw image 2

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }



    delete renderer;
    delete shader;
    delete va;
    delete vb;
    delete ib;

    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}