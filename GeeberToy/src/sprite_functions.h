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
    int footOffset; //pixels from top to bottom of shoe
};

// draw functions
void loadFrameFromSheet(TFT_eSprite &frame, const SpriteSheet &sheet, int frameIndex);
void drawSpriteFrame(TFT_eSprite &screen, TFT_eSprite &frameSprite, const SpriteSheet &sheet, int frameIndex, int x, int y, uint16_t transparentColor);