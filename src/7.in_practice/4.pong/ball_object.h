#ifndef BALLOBJECT_H
#define BALLOBJECT_H
#include <irrklang/irrKlang.h>
using namespace irrklang;

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"


class BallObject : public GameObject
{
public:
    float   Radius;
    glm::vec2 INITIAL_POSITION;

    bool leftPadCollision = false;
    bool rightPadCollision = false;

    // constructor(s)
    BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);
    glm::vec2 Move(float dt, unsigned int window_width, unsigned int window_height, float wallWidth, ISoundEngine* SoundEngine);

    static glm::vec2 RandomVelocity();
    static float VELOCITY_RANGE_X[2];
    static float VELOCITY_RANGE_Y[2];
};

#endif