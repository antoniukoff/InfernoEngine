#pragma once

#include <Vladgine/SpriteBatch.h>

#include "Ball.h"

// Ball renderer interface
class BallRenderer {
public:
     void renderBalls(Vladgine::SpriteBatch& spriteBatch, Ball& ball);
};