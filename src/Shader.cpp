#include "Shader.h"
#include <GL/glew.h>
#include "Renderer.h"
#include <sstream>
#include <fstream>
#include <iostream>



Shader::Shader(const std::string& filepath)
    :m_Filepath(filepath), m_RendererID(0)
{
    glCall(ShaderProgramSource source = ParseShader(filepath));
    glCall(m_RendererID = CreateShader(source.VertexSource, source.FragmentSource));

}


void Shader::Bind() const{
    glCall(glUseProgram(m_RendererID));
}
void Shader::UnBind() const {
    glCall(glUseProgram(0));
}

Shader::~Shader()
{
    glCall(glDeleteProgram(m_RendererID));
}

void Shader::SetUniforms4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    int location = GetUniformLocation(name);
    glCall(glUniform4f(location, v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    glCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Uniform " << name << " d  oes not exist." << std::endl;
    }
    else
        m_UniformLocationCache[name] = location;
    return location;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);


    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 5;
    }

    return id;

}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{

    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream sStream[2];
    ShaderType type = ShaderType::NONE;


    while (getline(stream, line)) {

        if (line.find("#shader") != std::string::npos) {

            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;

            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;

            }
            else {
                type = ShaderType::NONE;
            }

        }
        else {

            sStream[(int)type] << line << '\n';
        }

    }
    return{ sStream[0].str(),sStream[1].str() };


}


int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

