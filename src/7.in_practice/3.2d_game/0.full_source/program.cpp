/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "resource_manager.h"

#include <iostream>

// GLFW function declerations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The Width of the screen
const unsigned int SCREEN_WIDTH = 800;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;

GLFWmonitor* monitor;
bool isFullScreen = false;
int windowPos[2] = { 0, 0 };
int windowSize[2] = { SCREEN_WIDTH, SCREEN_HEIGHT };
int resolution[5][2] = { {800, 600}, {1024, 768}, {1280, 720}, {1920, 1080}, {2560, 1440} };
int nRes = 0;

Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    monitor = glfwGetPrimaryMonitor();

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    int joystickPresent = glfwJoystickPresent(GLFW_JOYSTICK_1);
    if (joystickPresent == GLFW_TRUE) {
        std::cout << "Joystick present" << std::endl;
        const char* name = glfwGetGamepadName(GLFW_JOYSTICK_1);
        std::cout << "Gamepad:" << name << std::endl;
    }
    else
        std::cout << "Joystick NOT present" << std::endl;

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialize game
    // ---------------
    Breakout.Init();
    Breakout.resW = resolution[0][0];
    Breakout.resH = resolution[0][1];

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // start game within menu state
    // ----------------------------
    Breakout.State = GAME_MENU;

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // joystick test
        if (joystickPresent) {
            int buttonCount;
            const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
            if (GLFW_PRESS == buttons[0]) {
                Breakout.gamepadButtonA = true;
                //std::cout << "A button pressed" << std::endl;
            }
            if (GLFW_PRESS == buttons[1]) {
                Breakout.gamepadButtonX = true;
                //std::cout << "A button released" << std::endl;
            }
        }

        int axesCount;
        const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
        // std::cout << "Axes:" << axesCount << std::endl;
        // std::cout << "Left X axis:" << axes[0] << std::endl;
        // std::cout << "Left Y axis:" << axes[1] << std::endl;

        Breakout.gamepadAxis = (float*)axes;
        // manage user input
        // -----------------
        Breakout.ProcessInput(deltaTime);

        // update game state
        // -----------------
        Breakout.Update(deltaTime);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Breakout.Render();

        glfwSwapBuffers(window);
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // F11 controls full screen toggle
    if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
    {
        std::cout << "F11 pressed:";
        if (isFullScreen)
        {
            std::cout << "Exiting full screen." << std::endl;
            std::cout << "windowsPos: [" << windowPos[0] << ", " << windowPos[1] << "]" << std::endl;
            std::cout << "windowsSize: [" << windowSize[0] << ", " << windowSize[1] << "]" << std::endl;
            // restore last window size and position
            glfwSetWindowMonitor(window, nullptr, windowPos[0], windowPos[1], windowSize[0], windowSize[1], GLFW_DONT_CARE);
        }
        else
        {
            std::cout << "Entering full screen." << std::endl;

            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            std::cout << "mode[W,H]: [" << mode->width << ", " << mode->height << "]" << std::endl;

            // backup window position and window size
            glfwGetWindowPos(window, &windowPos[0], &windowPos[1]);
            glfwGetWindowSize(window, &windowSize[0], &windowSize[1]);

            std::cout << "windowsPos: [" << windowPos[0] << ", " << windowPos[1] << "]" << std::endl;
            std::cout << "windowsSize: [" << windowSize[0] << ", " << windowSize[1] << "]" << std::endl;

            // Set full-screen
            //glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            glfwSetWindowMonitor(window, monitor, 0, 0, windowSize[0], windowSize[1], mode->refreshRate);
        }
        isFullScreen = !isFullScreen;
    }

    // F1 changes the resolution if in full screen
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        if (isFullScreen)
        {
            std::cout << "nRes:" << nRes << std::endl;
            Breakout.resW = resolution[nRes][0];
            Breakout.resH = resolution[nRes][1];

            // I don't really know what I am doing...
            glfwSetWindowSize(window, Breakout.resW, Breakout.resW);
            glViewport(0, 0, Breakout.resW, Breakout.resW);
            glfwSetWindowMonitor(window, monitor, 0, 0, Breakout.resW, Breakout.resH, GLFW_DONT_CARE);

            nRes++;
            if (nRes > 4)
                nRes = 0;

        }
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            Breakout.Keys[key] = true;
        else if (action == GLFW_RELEASE)
        {
            Breakout.Keys[key] = false;
            Breakout.KeysProcessed[key] = false;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.

    std::cout << "framebuffer_size_callback()" << std::endl;

    std::cout << "[width, height]: [" << width << ", " << height << "]" << std::endl;
    std::cout << "windowsPos: [" << windowPos[0] << ", " << windowPos[1] << "]" << std::endl;
    std::cout << "windowsSize: [" << windowSize[0] << ", " << windowSize[1] << "]" << std::endl;
    //windowSize[0] = width;
    //windowSize[1] = height;
    // Breakout.Width = width;
    // Breakout.Height = height;
    glViewport(0, 0, width, height);
}