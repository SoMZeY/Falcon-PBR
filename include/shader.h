#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);

    // Activate the shader
    void use() const;

    // Utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, glm::mat4 value) const;
    void setVec3(const std::string& name, glm::vec3 value) const;

    // Getting the uniform block name
    unsigned int getUniformBlockIndex(const std::string& name) const;
private:
    unsigned int m_Id;
};

#endif