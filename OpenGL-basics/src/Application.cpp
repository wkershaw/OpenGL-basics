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


struct ShaderSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderSource parseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {NONE=-1,VERTEX=0,FRAGMENT=1};

    ShaderType type = ShaderType::NONE;
    std::string line;
    std::stringstream ss[2];

    while(getline(stream,line)){
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;         
        }
        else {
            if(type != ShaderType::NONE)
                ss[(int)type] << line << '\n';
        }
    }

    return {ss[0].str(),ss[1].str()};
}

static unsigned int CompileShader(unsigned int type,const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); //allocate on stack dynamically
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(std::string& vertexShader, std::string & fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


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

    ShaderSource shaderSource = parseShader("res/shaders/basic.shader"); //Parse a simple shader using a file path

    unsigned int shader = CreateShader(shaderSource.VertexSource,shaderSource.FragmentSource); //Create the shader
    glUseProgram(shader); //Assign the shader to the OpenGL state machine

    int location = glGetUniformLocation(shader,"u_colour"); //Find the location of the uniform variable within the shader
    ASSERT(location != -1); //The location should not be -1, if so, the uiform cannot be found in the shader
    glUniform4f(location, 0.6f, 0.3f, 0.8f, 1.0f); //Set the values of the uniform


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
        glUniform4f(location, r, 0.3f, 0.8f, 1.0f); //Set the values of the uniform
        //Use GLCALL to check for errors
        GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,nullptr)); //Draw the vertices using the index buffer

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader); //Delete the shader once we've finished with it

    delete va;
    delete vb;
    delete ib;
    glfwTerminate();
    return 0;
}