/**
 * @file game.cpp
 * @brief Handles primary, high-level game logic
 * @date December 2023
 * @see https://learnopengl.com/In-Practice/2D-Game
 */

#include "game.h"

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height) {

}

Game::~Game() {

}

void Game::init() {

}

void Game::update(float dt) {

}

void Game::processInput(float dt) {

}

void Game::render() {

}

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/