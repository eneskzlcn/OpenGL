#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
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
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, int value);	
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	int GetUniformLocation(const std::string& name);
	inline unsigned int GetRendererID() { return m_RendererID; }
private:
	ShaderSource ParseShader(const std::string& filepath);
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
	std::string m_Filepath;

};

