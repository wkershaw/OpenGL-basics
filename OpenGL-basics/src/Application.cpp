#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
    
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }


    std::cout << "OpenGL loaded: " <<glGetString(GL_VERSION) << std::endl;

    float positions[] = { //Vertex positions for a square
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f
    };

    unsigned int indices[] = { //Inices to draw a square
        0,1,2,
        2,3,0
    };

    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCALL(glEnable(GL_BLEND))

    Texture* texture = new Texture("res/textures/colourSplash.png");
    texture->Bind();
    Shader* shader = new Shader("res/shaders/basic.shader"); //Create a new shader    
    shader->Bind();
    shader->SetUniform1i("u_Texture", 0); //Bind the texture to slot 0



    VertexArray* va = new VertexArray(); //Make a new vertex array object to store the VB and its layout
    VertexBuffer* vb = new VertexBuffer(positions, 4 * 4 * sizeof(float)); //Make a buffer to store the vertex positions
    
    
    VertexBufferLayout* layout = new VertexBufferLayout(); //Define the layout for the buffer
    layout->Push<float>(2); //Each position is made up of 2 floats
    layout->Push<float>(2);
    
    
    va->AddBuffer(*vb, *layout); //Update the vertex array object with the position data
    IndexBuffer* ib = new IndexBuffer(indices, 6); //Define which order the vertices should be drawn in
    
    Renderer* renderer = new Renderer(); //Create a new render to make draw calls

    float increment = 0.05f;
    float r = 0.0f;


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer->Clear();

        va->Bind();
        ib->Bind();

        if (r > 1.0f || r < 0.0f)
            increment *= -1;
        r += increment;

        shader->Bind();
        shader->SetUniform4f("u_colour", r, 0.3f, 0.8f, 1.0f); //Set the colour uniform to be passed into the shader
        renderer->Draw(*va, *ib, *shader);

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
    glfwTerminate();
    return 0;
}