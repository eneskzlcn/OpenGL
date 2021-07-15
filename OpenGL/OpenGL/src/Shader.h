#pragma once
#include <string>
#include <unordered_map>

struct ShaderSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();
	void Bind() const;
	void UnBind() const;
	ShaderSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	unsigned int GetUniformLocation(const std::string& name);
	inline unsigned int GetRendererID() { return m_RendererID; }
private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, unsigned int> m_UniformLocationCache;
	std::string m_Filepath;

};

