#pragma once
#include <Arduino.h>
#include "sprite_functions.h"

struct Player {
    int x;
    float y;
    float velocityY;
    bool onGround;
    int idleFrame;
    unsigned long lastIdleAnimTime;
};

void playerInit(Player &p);
void playerStartJump(Player &p);
void playerUpdate(Player &p, const SpriteSheet &idleSheet);
int playerGetJumpFrame(const Player &p);