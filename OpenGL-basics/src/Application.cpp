#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


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


    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }


    std::cout << "OpenGL loaded: " <<glGetString(GL_VERSION) << std::endl;

    float positions[] = { //Vertex positions for a triangle
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    unsigned int buffer; //Create a place to store the buffer ID
    glGenBuffers(1, &buffer); //Generate a buffer and return its ID
    glBindBuffer(GL_ARRAY_BUFFER, buffer); //Set the current buffer in the OpenGL state machine
    glBufferData(GL_ARRAY_BUFFER, 8 *sizeof(float), positions,GL_STATIC_DRAW); //Add position data to the buffer
    
    glEnableVertexAttribArray(0); //Enable an attribute for the buffer, defined on the next line
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float),(const void*)0); //Declare the layout of the buffer data

    unsigned int ibo; //Create an index buffer
    glGenBuffers(1, &ibo); //Generate the buffer and return its ID
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //Set the current buffer in the OpenGL state machine
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW); //Add index data to the buffer


    ShaderSource shaderSource = parseShader("res/shaders/basic.shader"); //Parse a simple shader

    unsigned int shader = CreateShader(shaderSource.VertexSource,shaderSource.FragmentSource); //Create the shader
    glUseProgram(shader); //Assign the shader to the OpenGL state machine


    //glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind the buffer

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, buffer); //Bind the buffer
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,nullptr); //Draw the vertices using the index buffer

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader); //Delete the shader once we've finished with it

    glfwTerminate();
    return 0;
}