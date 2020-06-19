#include "Renderer.h"

int main(void)
{
    Renderer* renderer = new Renderer(); //Initialise renderer and window
    renderer->EnableBlending();  
    renderer->EnableDepth();


    float positions[] = { //Vertex positions for a square
            -50, -50,  -50,
              0, -50, -100,
             50, -50,  -50,
              0,  50,  -75,
    };

    unsigned int indices[] = { //Inices to draw a square
        0,2,3,
        1,2,3,
        0,1,2,
        0,1,3,
    };

    Shader* shader = new Shader("res/shaders/basic.shader"); //Create a new shader    
    
    Object* object = new Object(positions, 20, indices, 12);   
    object->SetColour(glm::vec4(1, 0, 0, 1));
    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(renderer->window))
    {
        /* Render here */
        renderer->Clear();
        renderer->NewImGuiFrame();

        renderer->Draw(object,shader);
        
        renderer->DrawFrame();
    }


    delete object;
    delete shader;
    delete renderer;
    return 0;
}