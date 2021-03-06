#pragma once
#include "OPENGL.h"
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{

private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	int GetUniformLocation(const std::string& name);
	std::unordered_map<std::string, int> m_UniformLocationCache;


	ShaderSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(std::string& vertexShader, std::string& fragmentShader);

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;


	void SetUniform1i(const std::string& name, int value);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniform1f(const std::string& name, float v1);
	void SetUniform3f(const std::string& name, const glm::vec3& vector);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform3fv(const std::string& name, const std::vector<glm::vec3>& values);
	void SetUniform3fv(const std::string& name, float* values, int size);
};