#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"


Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{
    ShaderSource source = ParseShader(filepath);;
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCALL(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCALL(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCALL(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) { //Query the cache to reduce glGetUniformLocation calls
        return m_UniformLocationCache[name];
    }

    GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str())); //Find the location of the uniform variable within the shader
    if (location == -1) { //If the uniform cannot be found within the shader...
        std::cout << "Warning: uniform '" << name << "' does not exist within shader, ensure the correct shader is bound before setting its uniform" << std::endl;
    }

    m_UniformLocationCache[name] = location; //Add the location to the cache

    return location;
}


ShaderSource Shader::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

    ShaderType type = ShaderType::NONE; //Set the initial shader type to none
    std::string line;
    std::stringstream ss[2];

    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) { //Check if the shader type has changed
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            if (type != ShaderType::NONE)
                ss[(int)type] << line << '\n'; //Write the line to the shader string
        }
    }

    return { ss[0].str(),ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    GLCALL(unsigned int id = glCreateShader(type)); //Create a new shader
    GLCALL(const char* src = source.c_str());
    GLCALL(glShaderSource(id, 1, &src, nullptr));
    GLCALL(glCompileShader(id)); //Compile the shaders source

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); //Get the result of the compilation
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //Get the length of the error result
        char* message = (char*)alloca(length * sizeof(char)); //allocate on stack dynamically
        glGetShaderInfoLog(id, length, &length, message); //Get the error message
        std::cout << "Failed to compile shader" << std::endl;
        std::cout << message << std::endl; //Print the error message for failed compilation
        glDeleteShader(id); //Delete the shader
        return 0;
    }

    return id; //Return the id of the compiled shader
}


unsigned int Shader::CreateShader(std::string& vertexShader, std::string& fragmentShader)
{
    unsigned int program = glCreateProgram(); //Create a new shader program
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); //Compile the vertex shader
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); //Compile the fragment shader

    glAttachShader(program, vs); //Attach the vertex shader to the program
    glAttachShader(program, fs); ///Atach the fragment shader to the program
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs); //Delete the individual shaders now that the program has been made
    glDeleteShader(fs);

    return program; //Return the location of the shader program
}
