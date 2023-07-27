/**
 * @file shader_g.h
 * @brief Shader class supporting matrices and vectors
 * @date July 2023
 */

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    Shader(const Shader& other) = delete;
    Shader(Shader&& other) = delete;
    Shader& operator=(const Shader& rhs) = delete;
    Shader& operator=(Shader&& rhs) = delete;
    ~Shader();
    void use();
    unsigned int getID();
    // uniform utilities
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
private:
    static const unsigned int INFO_LOG_SIZE = 1024;
    unsigned int id;
    // private utility methods
    const char* readShaderCode(const char* shaderPath);
    unsigned int compileShader(GLenum shaderType, const char* shaderCode);
    void checkCompileErrors(GLuint shader, GLenum shaderType = GL_NONE);
    std::string shaderTypeToString(GLenum shaderType);
};