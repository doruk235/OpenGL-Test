#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:

	std::string m_Filepath;
	unsigned int m_RendererID;
	//caching system for uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;

	//set uniform
	void SetUniforms4f(const std::string& name, float v0, float v1, float v2, float v3);

	int GetUniformLocation(const std::string& name);


private:
	
	unsigned int CompileShader(unsigned int type, const std::string& source);

	ShaderProgramSource ParseShader(const std::string& filepath);

	int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};