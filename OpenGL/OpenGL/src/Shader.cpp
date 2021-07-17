#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.h"

Shader::Shader(const std::string& filepath)

	:m_Filepath(filepath),m_RendererID(0)
{
	ShaderSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.vertexSource, source.fragmentSource); // create shader from a string 
}

Shader::~Shader()
{
	GLCALL(glDeleteShader(m_RendererID));
}

void Shader::Bind() const
{
	GLCALL(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
	GLCALL(glUseProgram(0));
}

ShaderSource Shader::ParseShader(const std::string& filepath)
{
	std::fstream stream(filepath);

	std::string line;

	std::stringstream ss[2];

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType currentShader = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				currentShader = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				currentShader = ShaderType::FRAGMENT;

			}
		}
		else
		{
			ss[(int)currentShader] << line << '\n';
		}
	}
	return { ss[0].str(),ss[1].str() };
}
 unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr); // add source code to tha shader source
	glCompileShader(id); // compile sourced shader program


	/* debugging and error checking stuffs.*/
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(sizeof(char) * length);//alloca gives a memory from stack dynamically :))
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compiled" << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	/* debugging and error checking stuffs finished.*/
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
{
	unsigned int program = glCreateProgram(); // request a free program from gpu returns the id of the program

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSource);  //returns compiled shader programs id
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER,fragmentSource);

	glAttachShader(program, vs); // attach shaders to free program
	glAttachShader(program, fs);

	glLinkProgram(program); // link the objects of program

	glValidateProgram(program); // validate the program for gpu

	glDeleteShader(vs); // delete intermediate files...
	glDeleteShader(fs);

	return program;
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCALL(glUniform4f(GetUniformLocation(name),v0,v1,v2,v3 ));

}

void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);	
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLCALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "[Warning!]  :" << name << "does not exist!" << std::endl;

	m_UniformLocationCache[name] = location;
	return location;

}

static ShaderSource ParseShader(const std::string& filepath)
{
	std::fstream stream(filepath);

	std::string line;

	std::stringstream ss[2];

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType currentShader = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				currentShader = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				currentShader = ShaderType::FRAGMENT;

			}
		}
		else
		{
			ss[(int)currentShader] << line << '\n';
		}
	}
	return { ss[0].str(),ss[1].str() };
}
static unsigned int CompileShader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr); // add source code to tha shader source
	glCompileShader(id); // compile sourced shader program


	/* debugging and error checking stuffs.*/
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(sizeof(char) * length);//alloca gives a memory from stack dynamically :))
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compiled" << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	/* debugging and error checking stuffs finished.*/
	return id;
}

static unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
{
	unsigned int program = glCreateProgram(); // request a free program from gpu returns the id of the program

	unsigned int vs = CompileShader(vertexSource, GL_VERTEX_SHADER);  //returns compiled shader programs id
	unsigned int fs = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs); // attach shaders to free program
	glAttachShader(program, fs);

	glLinkProgram(program); // link the objects of program

	glValidateProgram(program); // validate the program for gpu

	glDeleteShader(vs); // delete intermediate files...
	glDeleteShader(fs);

	return program;
}
