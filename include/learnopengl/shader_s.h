/**
 * @file shader_s.h
 * @brief Shader class
 * @date Created: July 2023 | Last modified: November 2024
 * @see https://learnopengl.com/Getting-started/Shaders
 */

#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
private:
	unsigned int id;
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	Shader(const Shader& other) = delete;
	Shader(Shader&& other) = delete;
	Shader& operator=(const Shader& other) = delete;
	Shader& operator=(Shader&& other) = delete;
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};

#endif