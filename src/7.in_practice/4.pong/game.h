/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H
#include <glm/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_object.h"
#include "ball_object.h"

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};
typedef std::tuple<bool, Direction, glm::vec2> Collision;

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState               State;
    bool                    paused = false;

    float                   pausedTime = 0.0f;
    float                   elapsedTime = 0.0f;
    long                    pausedFrames = 0;
    long                    elapsedFrames = 0;

    bool                    Keys[1024];
    bool                    KeysProcessed[1024];
    bool                    GamePadPresent[4];
    float*                  gamepadAxis1;
    float*                  gamepadAxis2;
    float                   prevGamepadAxis1[2];
    float                   prevGamepadAxis2[2];
    float                   diffGamepadAxis;

    float prevLeftPadX;
    float diffLeftPadX;

    unsigned int            Width;
    unsigned int            Height;
    float wallWidth = Height/25.0f;
    glm::vec2 PADSIZE = glm::vec2(10.0f, 50.0f);
    glm::vec2 PAD_VELOCITY = glm::vec2(800.0f, 1000.0f);
    float scaleFactor = 3.0f;

    GameObject* leftPad;
    GameObject* rightPad;
    bool pauseOnPadCollision = false;
    const float defaultLeftPadLimitX = Width / 4.0f;
    float leftPadLimitX = defaultLeftPadLimitX;
    const float defaultRightPadLimitX = 3.0f * Width / 4.0f - PADSIZE.x;
    float rightPadLimitX = defaultRightPadLimitX;
    

    static float percentage;
    static float distance;
    static float centerBoard;

    float BALL_RADIUS = 10.0f;
    BallObject* ball;

    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init(bool gp[4]);
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void PaintLetterBox(int fsWidth, int fsHeight, int fsStartX);

    Direction VectorDirection(glm::vec2 target);
    Collision CheckCollision(BallObject& ball, GameObject& pad);
    void AdjustBallVelocity(BallObject& ball, GameObject& pad);
};

#endif