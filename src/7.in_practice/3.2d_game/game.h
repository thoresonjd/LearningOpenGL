/**
 * @file game.h
 * @brief Handles primary, high-level game logic
 * @date December 2023
 * @see https://learnopengl.com/In-Practice/2D-Game
 */

#ifndef GAME_H
#define GAME_H
#pragma once

/**
 * Represents the current state of the game
 */
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

/**
 * @class Game - holds all game-related state and functionality. Combines all
 * game-related data into a single class for easy access to each of
 * the components and manageability.
 */
class Game {
public:
    // game state
    GameState               State;
    bool                    Keys[1024];
    unsigned int            Width, Height;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void init();
    // game loop
    void processInput(float dt);
    void update(float dt);
    void render();
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