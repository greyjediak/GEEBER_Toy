// Handles drawing sprites from the sheets in sprites.h

#include "sprite_functions.h"
#include <stdio.h>
using namespace std;
#include <TFT_eSPI.h>
#include "sprites.h"
#include  "jokes.h"

#define JUMP_FRAMES 4
#define JUMP_SHEET_W 256 // 64*4

void loadFrameFromSheet(TFT_eSprite &frame, const SpriteSheet &sheet, int frameIndex)
{
    int sourceX = frameIndex * sheet.frameW;

    for (int row = 0; row < sheet.frameH; row++)
    {
        const uint16_t *rowPtr = &sheet.data[row * sheet.sheetW + sourceX];
        frame.pushImage(0, row, sheet.frameW, 1, rowPtr);
    }
}

void drawSpriteFrame(TFT_eSprite &screen, TFT_eSprite &frameSprite, const SpriteSheet &sheet, int frameIndex, int x, int y, uint16_t transparent)
{
    frameSprite.fillSprite(transparent);
    loadFrameFromSheet(frameSprite, sheet, frameIndex);
    frameSprite.pushToSprite(&screen, x, y, transparent);
}