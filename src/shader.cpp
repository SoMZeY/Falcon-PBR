#include "shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    if (!std::filesystem::exists(vertexPath))
        std::cerr << "Vertex shader file not found: " << vertexPath << "\n";

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // Open Files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        // Read file contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // Close filie handlers
        vShaderFile.close();
        fShaderFile.close();

        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // Compile shaders
    int success;
    char infoLog[512];

    // Vertex Shader
    // Compile vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    // Check for any compilation errors
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error on vertex shader compilation: " << infoLog << std::endl;
    }

    // Compile
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error on fragment shader compilation: " << infoLog << std::endl;
    }

    // Create shader program
    m_Id = glCreateProgram();
    glAttachShader(m_Id, vertexShader);
    glAttachShader(m_Id, fragmentShader);
    glLinkProgram(m_Id);
    glGetProgramiv(m_Id, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(m_Id, 512, NULL, infoLog);
        std::cout << "Error linking shader program: " << infoLog << std::endl;
    }

    // Delete the shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() const
{
    glUseProgram(m_Id);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_Id, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(m_Id, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(m_Id, name.c_str()), 1, glm::value_ptr(value));
}

unsigned int Shader::getUniformBlockIndex(const std::string& name) const
{
    return glGetUniformBlockIndex(m_Id, name.c_str());
}

void Shader::glBindBlockToBindingPoint(unsigned int blockIdx, unsigned int uniformBlockBinding) const
{
    glUniformBlockBinding(m_Id, blockIdx, uniformBlockBinding);
}
