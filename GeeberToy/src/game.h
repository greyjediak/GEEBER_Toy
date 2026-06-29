#pragma once
#include <Arduino.h>
#include "sprite_functions.h"
#include "graphics.h"

// game start and drawing handling
void gameInit();
bool gameUpdate(bool jumpPressed, bool exitPressed); // to jump, use the right button. To return to the main menu, left
void gameDraw();

// Player related functions
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
