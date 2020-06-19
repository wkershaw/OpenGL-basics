#include "Renderer.h"

int main(void)
{
    Renderer* renderer = new Renderer(); //Initialise renderer and window
    renderer->EnableBlending();  
    renderer->EnableDepth();


    float positions[] = { //Vertex positions for a square
            -1, -1, -1,
             1, -1, -1,
            -1, -1,  1,
             1, -1,  1,
            -1,  1, -1,
             1,  1, -1,
            -1,  1,  1,
             1,  1,  1,
    };

    unsigned int indices[] = { //Inices to draw a square
        0,1,5,
        0,4,5,
        4,6,7,
        4,5,7,
        2,3,7,
        2,6,7,
        0,2,3,
        0,1,3,
        0,2,4,
        6,2,4,
        1,3,5,
        7,3,5
    };

    Shader* shader = new Shader("res/shaders/basic.shader"); //Create a new shader    
    
    Object* object = new Object(positions, 24, indices, 36);   
    object->SetColour(glm::vec4(1, 0, 0, 1));
    
    glm::vec3 objectTranslation = glm::vec3(0, 0, -100);
    glm::vec3 objectRotation = glm::vec3(0, 0, -0);
    glm::vec3 objectScale = glm::vec3(1, 1, 1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(renderer->window))
    {
        /* Render here */
        renderer->Clear();
        renderer->NewImGuiFrame();

        ImGui::SliderFloat3("Object Translation", &objectTranslation.x, -30.0f, 30.0f); //Set the translation for image 1
        ImGui::SliderFloat3("Object Rotation", &objectRotation.x, 0.0f, 6.0f); //Set the translation for image 1
        ImGui::SliderFloat3("Object Scale", &objectScale.x, -5.0f, 5.0f); //Set the translation for image 1

        object->SetTranslation(objectTranslation);
        object->SetRotation(objectRotation);
        object->SetScale(objectScale);

        renderer->Draw(object,shader);
        
        renderer->DrawFrame();
    }


    delete object;
    delete shader;
    delete renderer;
    return 0;
}