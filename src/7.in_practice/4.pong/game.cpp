#include "filesystem.h"
#include <irrklang/irrKlang.h>
using namespace irrklang;

#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

#include <iostream>
#include <tuple>
#include <cstdlib>
#include <ctime>

ISoundEngine* SoundEngine = createIrrKlangDevice();

// Game-related State data
SpriteRenderer* Renderer;
SpriteRenderer* RepeatRenderer;

float Game::percentage = 0.0f;
float Game::centerBoard = 0.0f;
float Game::distance = 0.0f;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{}

Game::~Game()
{
    delete Renderer;
    SoundEngine->drop();
}

void Game::Init(bool gp[4])
{
    srand(static_cast <unsigned> (time(0)));

    for (int i = 0; i < 4; i++) {
        GamePadPresent[i] = gp[i];
    }

    Texture2D tBall, tPad;
    { // Shaders & textures
        ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.fs", nullptr, "sprite");
        glm::mat4 projection = glm::ortho(0.0f, (float)Width, (float)Height, 0.0f, -1.0f, 1.0f);
        ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
        ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
        // set render-specific controls
        Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
        RepeatRenderer = new SpriteRenderer(ResourceManager::GetShader("sprite"), true);

        tBall = ResourceManager::LoadTexture("textures/tenis.png", true, "face");
        tPad = ResourceManager::LoadTexture("textures/paddle.png", true, "pad");
        ResourceManager::LoadTexture("textures/wall2.png", false, "wall");
        ResourceManager::LoadTexture("textures/net.png", true, "net");
    }

    { // Game objects
        float padsInitialY = Height / 2.0f - scaleFactor * PADSIZE.y / 2.0f;
        glm::vec2 leftPadPos = glm::vec2(0.0f, padsInitialY);
        leftPad = new GameObject(leftPadPos, scaleFactor * PADSIZE, tPad, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::vec2 rightPadPos = glm::vec2(Width - scaleFactor * PADSIZE.x, padsInitialY);
        rightPad = new GameObject(rightPadPos, scaleFactor * PADSIZE, tPad, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::vec2 ballPos = glm::vec2(Width / 2.0f, Height / 2.0f);
        ball = new BallObject(ballPos, BALL_RADIUS, BallObject::RandomVelocity(), tBall);
    }

    SoundEngine->play2D(FileSystem::getPath("audio/tranquila.mp3").c_str(), true);
}

void Game::ProcessInput(float dt)
{
    glm::vec2 velocity = PAD_VELOCITY * dt;
    const float* axes;
    int axesCount;

    // TODO: Velocidad dependiente del Axis.
    // Más cerca del axis 1.0/-1.0, la velocidad aumenta más rápidamente.
    // Sería una especie de aceleración

    // Joystick1 - leftPad
    {
        axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
        gamepadAxis1 = (float*)axes;
        if (this->Keys[GLFW_KEY_W] || (gamepadAxis1[1] < -0.1f)) {
            if (leftPad->Position.y >= 0.0f + wallWidth) {
                leftPad->Position.y -= velocity.y;
            }
        }
        if (this->Keys[GLFW_KEY_S] || (gamepadAxis1[1] > 0.1f)) {
            if (leftPad->Position.y <= Height - leftPad->Size.y - wallWidth) {
                leftPad->Position.y += velocity.y;
            }
        }
        if (this->Keys[GLFW_KEY_D] || (gamepadAxis1[0] > 0.1f)) {
            if (leftPad->Position.x >= 0.0f && leftPad->Position.x < leftPadLimitX) {
                leftPad->Position.x += velocity.x;
                if (leftPad->Position.x >= leftPadLimitX) {
                    leftPad->Position.x = leftPadLimitX;
                }
            }
        }
        if (this->Keys[GLFW_KEY_A] || (gamepadAxis1[0] < -0.1f)) {
            if (leftPad->Position.x > 0.0f && leftPad->Position.x <= leftPadLimitX) {
                leftPad->Position.x -= velocity.x;
                if (leftPad->Position.x < 0.0f) {
                    leftPad->Position.x = 0.0f;
                }
            }
        }
    }

    // Joystick 2 - rightPad
    {
        axes = glfwGetJoystickAxes(GLFW_JOYSTICK_2, &axesCount);
        gamepadAxis2 = (float*)axes;
        if (this->Keys[GLFW_KEY_UP] || (gamepadAxis2[1] < -0.1f))
        {
            if (rightPad->Position.y >= 0.0f + wallWidth)
            {
                rightPad->Position.y -= velocity.y;
            }
        }
        if (this->Keys[GLFW_KEY_DOWN] || (gamepadAxis2[1] > 0.1f))
        {
            if (rightPad->Position.y <= Height - rightPad->Size.y - wallWidth)
            {
                rightPad->Position.y += velocity.y;
            }
        }
        if (this->Keys[GLFW_KEY_LEFT] || (gamepadAxis2[0] < -0.1f))
        {
            if (rightPad->Position.x <= Width - rightPad->Size.x && rightPad->Position.x > rightPadLimitX)
            {
                rightPad->Position.x -= velocity.x;
                if (rightPad->Position.x < rightPadLimitX) {
                    rightPad->Position.x = rightPadLimitX;
                }
            }
        }
        if (this->Keys[GLFW_KEY_RIGHT] || (gamepadAxis2[0] > 0.1f))
        {
            if (rightPad->Position.x <= Width - rightPad->Size.x && rightPad->Position.x >= rightPadLimitX)
            {
                rightPad->Position.x += velocity.x;
                if (rightPad->Position.x >= Width - rightPad->Size.x) {
                    rightPad->Position.x = Width - rightPad->Size.x;
                }
            }
        }
    }
}

Direction Game::VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

//bool CheckCollision(BallObject& ball, GameObject& pad)
Collision Game::CheckCollision(BallObject& ball, GameObject& pad)
{
    glm::vec2 center(ball.Position + ball.Radius);
    glm::vec2 aabb_half_extents(pad.Size.x / 2.0f, pad.Size.y / 2.0f);
    glm::vec2 aabb_center(
        pad.Position.x + aabb_half_extents.x,
        pad.Position.y + aabb_half_extents.y
    );
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    glm::vec2 closest = aabb_center + clamped;
    difference = closest - center;

    //return glm::length(difference) < ball.Radius;
    if (glm::length(difference) <= ball.Radius)
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

void Game::AdjustBallVelocity(BallObject& ball, GameObject& pad)
{
    Game::centerBoard = pad.Position.y + pad.Size.y / 2.0f;
    float ballPosY;
    if (ball.Position.y < Game::centerBoard) {
        ballPosY = ball.Position.y + ball.Radius;
    }
    else {
        ballPosY = ball.Position.y;
    }

    // TODO: Comprobar que el porcentaje no es mayor que 1.0 ... no tiene sentido
    Game::distance = abs(ballPosY - Game::centerBoard);
    Game::percentage = Game::distance / (pad.Size.y / 2.0f);

    float strength = 3.0f;
    float velocity = abs(ball.Velocity.x) * Game::percentage * strength;

    if (velocity < 500.0f)
        velocity = 500.0f;
    if (velocity > 2000.0f)
        velocity = 2000.0f;

    if (ball.Velocity.x > 0)
        ball.Velocity.x = -velocity;
    else
        ball.Velocity.x = velocity;

}

void Game::Update(float dt)
{
    ball->Move(dt, Width, Height, wallWidth, SoundEngine);

    if (ball->leftPadCollision || ball->rightPadCollision) {
        if (ball->Position.x > 1.0f * Width / 4.0f && ball->Position.x < 3.0f * Width / 4.0f) {
            ball->leftPadCollision = ball->rightPadCollision = false;
            leftPadLimitX = defaultLeftPadLimitX;
            rightPadLimitX = defaultRightPadLimitX;
        }
    }

    Collision collisionLeftPad = CheckCollision(*ball, *leftPad);
    if (std::get<0>(collisionLeftPad)) {
        ball->leftPadCollision = true;
        if (pauseOnPadCollision) {
            paused = true;
        }

        diffGamepadAxis = gamepadAxis1[0] - prevGamepadAxis1[0];
        diffLeftPadX = leftPad->Position.x - prevLeftPadX;
        AdjustBallVelocity(*ball, *leftPad);
        ball->Position.x = leftPad->Position.x + leftPad->Size.x;

        leftPadLimitX = leftPad->Position.x;
        SoundEngine->play2D(FileSystem::getPath("audio/bleep.mp3").c_str(), false);
    }

    Collision collisionRightPad = CheckCollision(*ball, *rightPad);
    if (std::get<0>(collisionRightPad)) {
        ball->rightPadCollision = true;
        if (pauseOnPadCollision) {
            paused = true;
        }
        AdjustBallVelocity(*ball, *rightPad);
        ball->Position.x = rightPad->Position.x - ball->Size.x;


        SoundEngine->play2D(FileSystem::getPath("audio/bleep.mp3").c_str(), false);
    }
}

void Game::Render()
{
    Texture2D tWall = ResourceManager::GetTexture("wall");
    Renderer->DrawSprite(tWall, glm::vec2(0.0f, 0.0f), glm::vec2((float)Width, wallWidth));
    Renderer->DrawSprite(tWall, glm::vec2(0.0f, (float)Height - wallWidth), glm::vec2((float)Width, wallWidth));

    Texture2D tNet = ResourceManager::GetTexture("net");
    float midScreen = Width / 2.0f;
    RepeatRenderer->DrawSprite(tNet, glm::vec2(midScreen - midScreen / 100.0f, wallWidth), glm::vec2(2.0f * midScreen / 100.0f, (float)Height - wallWidth));

    leftPad->Draw(*Renderer);
    rightPad->Draw(*Renderer);
    ball->Draw(*Renderer);
}

void Game::PaintLetterBox(int fsWidth, int fsHeight, int fsStartX)
{
    Texture2D tAny = ResourceManager::GetTexture("wall");
    Renderer->DrawSprite(tAny, glm::vec2(-(float)fsStartX, 0.0f), glm::vec2(0.0f, fsHeight), 0.0f, glm::vec3(0.0f, 0.0f, 0.0f));
}

