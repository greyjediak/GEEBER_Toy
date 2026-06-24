#pragma once

#include <stdio.h>
using namespace std;
#include <TFT_eSPI.h>
#include "sprites.h"
#include  "jokes.h"

struct SpriteSheet {
    const uint16_t *data;
    int frameW;
    int frameH;
    int frameCount;
    int sheetW;
};

// draw functions
void loadFrameFromSheet(TFT_eSprite &frame, const SpriteSheet &sheet, int frameIndex);
void drawSpriteFrame(TFT_eSprite &screen, TFT_eSprite &frameSprite, const SpriteSheet &sheet, int frameIndex, int x, int y, uint16_t transparentColor);

// action functions
void sprite_idle(TFT_eSprite, int xPos, int yPos) {};
void sprite_jump(TFT_eSprite sprite, int start, int end, int jump_height, int jump_speed) {};
void sprite_walk_left(TFT_eSprite sprite, int start, int end, int speed) {};
void sprite_walk_right(TFT_eSprite sprite, int start, int end, int speed) {};
void sprite_tell_joke(TFT_eSprite sprite, string joke);