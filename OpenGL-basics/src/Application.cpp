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
    const float PI = 3.14159265359f;
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

Object* CreatePlane() {
    float positions[] = { //Vertex positions for a square
        -1, -1, 00, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         1, -1, 00, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         1,  1, 00, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1,  1, 00, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = { //Inices to draw a square
        0,1,2,
        2,3,0
    };

    Object* obj = new Object(positions, 8 * 4, indices, 6);
    return obj;
}


Material* Metal() {
    Material* metal = new Material();
    metal->AddTexture(new Texture("u_albedoMap", "res/textures/metal/albedo.png"));
    metal->AddTexture(new Texture("u_metallicMap", "res/textures/metal/metallic.png"));
    metal->AddTexture(new Texture("u_normalMap", "res/textures/metal/normal.png"));
    metal->AddTexture(new Texture("u_roughnessMap", "res/textures/metal/roughness.png"));
    metal->AddTexture(new Texture("u_aoMap", "res/textures/metal/ao.png"));

    return metal;
}

Material Rock() {
    Material rock = *new Material();
    rock.AddTexture(new Texture("u_albedoMap", "res/textures/paint/albedo.png"));
    rock.AddTexture(new Texture("u_metallicMap", "res/textures/paint/metallic.png"));
    rock.AddTexture(new Texture("u_normalMap", "res/textures/paint/normal.png"));
    rock.AddTexture(new Texture("u_roughnessMap", "res/textures/paint/roughness.png"));
    rock.AddTexture(new Texture("u_aoMap", "res/textures/paint/ao.png"));

    return rock;
}

int main(void)
{
    Renderer* renderer = new Renderer(); //Initialise renderer and window
    std::cout << "OpenGL loaded: " << glGetString(GL_VERSION) << std::endl;
    renderer->EnableBlending();  
    renderer->EnableDepth();
    renderer->StartImGui();

    Shader* basicshader = new Shader("res/shaders/basic.shader");
    Shader* PBRshader = new Shader("res/shaders/PBR.shader"); //Create a new shader      
    Material* metalMaterial = Metal();
    
    Object* object = CreateSphere();
    object->AddMaterial(metalMaterial);
    object->SetTranslation(glm::vec3(0, 0, -6));

    glm::vec3 lightTranslation = glm::vec3(-1, 1, -2);
    glm::vec3 lightColour = glm::vec3(200, 200, 200);
    Light* light = new Light(lightTranslation, lightColour);
    Object* lightObject = CreateSphere();
    lightObject->SetScale(glm::vec3(0.2f));
    renderer->AddLight(light);

    /* Loop until the user closes the window */
    while (!(renderer->WindowShouldClose()))
    {
        /* Render here */
        renderer->Clear();
        renderer->NewImGuiFrame();

        ImGui::SliderFloat3("Light Position", &lightTranslation.x, -3.0f, 3.0f);
        ImGui::SliderFloat3("Light Colour", &lightColour.r, 0.0f, 400.0f);

        light->SetColour(lightColour);
        light->SetPosition(lightTranslation);

        lightObject->SetTranslation(lightTranslation);

        renderer->Draw(object, PBRshader);
        renderer->Draw(lightObject, basicshader);

        renderer->DrawFrame();
    }


    delete object;
    delete PBRshader;
    delete renderer;
    return 0;
}
