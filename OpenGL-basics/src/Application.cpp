#include "Renderer.h"
#include "Texture.h"

Object* CreateSphere() {

    unsigned int indexCount;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;


    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359;
    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
    {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            positions.push_back(glm::vec3(xPos, yPos, zPos));
            uv.push_back(glm::vec2(xSegment, ySegment));
            normals.push_back(glm::vec3(xPos, yPos, zPos));
        }
    }

    bool oddRow = false;
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else
        {
            for (int x = X_SEGMENTS; x >= 0; --x)
            {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }
    indexCount = indices.size();

    std::vector<float> data;
    for (std::size_t i = 0; i < positions.size(); ++i)
    {
        data.push_back(positions[i].x);
        data.push_back(positions[i].y);
        data.push_back(positions[i].z);
        if (uv.size() > 0)
        {
            data.push_back(uv[i].x);
            data.push_back(uv[i].y);
        }
        if (normals.size() > 0)
        {
            data.push_back(normals[i].x);
            data.push_back(normals[i].y);
            data.push_back(normals[i].z);
        }
    }


    Object* sphere = new Object(data.data(), data.size(), indices.data(), indices.size());
    sphere->SetDrawMode(Object::DrawMode::TRIANGLE_STRIP);

    return sphere;
}

Material* Metal() {
    Material* metal = new Material();
    metal->AddTexture(new Texture("u_albedoMap", "res/textures/albedo.png"));
    metal->AddTexture(new Texture("u_metallicMap", "res/textures/metallic.png"));
    metal->AddTexture(new Texture("u_normalMap", "res/textures/normal.png"));
    metal->AddTexture(new Texture("u_roughnessMap", "res/textures/roughness.png"));
    metal->AddTexture(new Texture("u_aoMap", "res/textures/ao.png"));

    return metal;
}

int main(void)
{
    Renderer* renderer = new Renderer(); //Initialise renderer and window
    renderer->EnableBlending();  
    renderer->EnableDepth();
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    Shader* PBRshader = new Shader("res/shaders/PBR.shader"); //Create a new shader    
    Shader* basicShader = new Shader("res/shaders/basic.shader"); //Create a new shader    
    
    Object* object = CreateSphere();
    Object* light = CreateSphere();

    Material* PBRmaterial = Metal();


    object->AddMaterial(PBRmaterial);
    object->AddLight();

    glm::vec3 objectTranslation = glm::vec3(0, 0, -4);
    glm::vec3 objectRotation = glm::vec3(0, 0, -0);
    glm::vec3 objectScale = glm::vec3(1, 1, 1);

    glm::vec3 lightTranslation = glm::vec3(5., 5, 2);
    glm::vec3 lightColour = glm::vec3(300,300,300);
    glm::vec3 lightScale = glm::vec3(0.3f, 0.3f, 0.3f);
    light->SetScale(lightScale);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(renderer->window))
    {
        /* Render here */
        renderer->Clear();
        renderer->NewImGuiFrame();

        ImGui::SliderFloat3("Object Translation", &objectTranslation.x, -30.0f, 30.0f); //Set the translation for the object
        ImGui::SliderFloat3("Object Rotation", &objectRotation.x, 0.0f, 6.0f); //Set the rotation for the object
        ImGui::SliderFloat3("Object Scale", &objectScale.x, 0.0f, 10.0f); //Set the scale for the object
        ImGui::SliderFloat3("Light Position", &lightTranslation.x, -20.0f, 20.0f); //Set the scale for the object
        ImGui::SliderFloat3("Light Colour", &lightColour.x, 0.0f, 400.0f); //Set the scale for the object



        object->SetTranslation(objectTranslation);
        object->SetRotation(objectRotation);
        object->SetScale(objectScale);
        object->SetLightPosition(0,lightTranslation);
        object->SetLightColour(0, lightColour);

        light->SetTranslation(lightTranslation);

        renderer->Draw(object, PBRshader);
        renderer->Draw(light, basicShader);

        renderer->DrawFrame();
    }


    delete object;
    delete PBRshader;
    delete basicShader;
    delete renderer;
    return 0;
}
