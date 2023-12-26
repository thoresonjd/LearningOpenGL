/**
 * @file resource_manager.h
 * @brief Manages game resources
 * @date December 2023
 * @see https://learnopengl.com/In-Practice/2D-Game
 */

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#pragma once

#include <glad/glad.h>
#include <map>
#include <string>
#include "shader.h"
#include "texture.h"

/**
 * @class ResourceManager - A static singleton ResourceManager class that hosts
 * several functions to load Textures and Shaders. Each loaded texture and/or
 * shader is also stored for future reference by string handles. All functions
 * and resources are static and no public constructor is defined.
 */
class ResourceManager {
private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() {}
    // loads and generates a shader from file
    static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
    // loads a single texture from file
    static Texture2D loadTextureFromFile(const char* file, bool alpha);
public:
    // resource storage
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture2D> textures;
    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
    // retrieves a stored sader
    static Shader getShader(std::string name);
    // loads (and generates) a texture from file
    static Texture2D loadTexture(const char* file, bool alpha, std::string name);
    // retrieves a stored texture
    static Texture2D getTexture(std::string name);
    // properly de-allocates all loaded resources
    static void clear();
};

#endif

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/