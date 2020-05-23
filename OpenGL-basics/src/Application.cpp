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
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[] = { //Inices to draw a square
        0,1,2,
        2,3,0
    };


    VertexArray* va = new VertexArray(); //Make a new vertex array object to store the VB and its layout
    VertexBuffer* vb = new VertexBuffer(positions, 8 * sizeof(float)); //Make a buffer to store the vertex positions
    VertexBufferLayout* layout = new VertexBufferLayout(); //Define the layout for the buffer
    layout->Push<float>(2); //Each position is made up of 2 floats
    va->AddBuffer(*vb, *layout); //Update the vertex array object with the position data
    IndexBuffer* ib = new IndexBuffer(indices, 6); //Define which order the vertices should be drawn in
    Shader* shader = new Shader("res/shaders/basic.shader"); //Create a new shader    
    shader->Bind();

    float increment = 0.05f;
    float r = 0.0f;


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        va->Bind();
        ib->Bind();

        if (r > 1.0f || r < 0.0f)
            increment *= -1;
        r += increment;

        shader->Bind();
        shader->SetUniform4f("u_colour", r, 0.3f, 0.8f, 1.0f); //Set the colour uniform to be passed into the shader
        GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,nullptr)); //Draw the vertices using the index buffer

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    delete shader;
    delete va;
    delete vb;
    delete ib;
    glfwTerminate();
    return 0;
}