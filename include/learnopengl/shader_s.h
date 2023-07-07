/**
 * @file shader_s.h
 * @brief Shader class
 * @date July 2023
 * @see https://learnopengl.com/Getting-started/Shaders
 */

#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	void use();
	unsigned int getID();
	// uniform utilities
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
private:
	unsigned int id;
};