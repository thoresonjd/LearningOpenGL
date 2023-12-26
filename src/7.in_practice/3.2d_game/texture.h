/**
 * @file texture.h
 * @brief Handles texture loading
 * @date December 2023
 * @see https://learnopengl.com/In-Practice/2D-Game
 */

#ifndef TEXTURE_H
#define TEXTURE_H
#pragma once

#include <glad/glad.h>

/**
 * @class Texture2D - can store and configure a texture in OpenGL.
 * It also hosts utility functions for easy management.
 */
class Texture2D {
public:
    // holds the ID of the texture object, used for all texture operations to reference to this particular texture
    unsigned int id;
    // texture image dimensions
    unsigned int width, height; // width and height of loaded image in pixels
    // texture Format
    unsigned int internalFormat; // format of texture object
    unsigned int imageFormat; // format of loaded image
    // texture configuration
    unsigned int wrapS; // wrapping mode on S axis
    unsigned int wrapT; // wrapping mode on T axis
    unsigned int filterMin; // filtering mode if texture pixels < screen pixels
    unsigned int filterMax; // filtering mode if texture pixels > screen pixels
    // constructor (sets default texture modes)
    Texture2D();
    // generates texture from image data
    void generate(unsigned int width, unsigned int height, unsigned char* data);
    // binds the texture as the current active GL_TEXTURE_2D texture object
    void bind() const;
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