#pragma once
#include <string>
#include <unordered_map>

struct ShaderSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{

private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	unsigned int GetUniformLocation(const std::string& name);
	std::unordered_map<std::string, int> m_UniformLocationCache;


	ShaderSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(std::string& vertexShader, std::string& fragmentShader);

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;


	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
};