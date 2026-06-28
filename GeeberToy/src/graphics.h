#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>

constexpr int SCREEN_W = 240;
constexpr int SCREEN_H = 240;
constexpr uint16_t TRANSPARENT = 0xF81F;

constexpr int GROUND_Y = 152;
constexpr int PLAYER_H = 64;
constexpr int PLAYER_W = 64;

extern TFT_eSPI tft;
extern TFT_eSprite screen;
extern TFT_eSprite frameSprite;

void graphicsInit();
void beginFrame(uint16_t bgColor);
void endFrame();

// Text handlers
int drawText(
    const String &text,
    int x,
    int y,
    uint16_t color = TFT_BLACK,
    uint8_t size = 2,
    uint16_t maxWidth = 220
);
void drawCenteredText(
    const String &text,
    int y,
    uint16_t color = TFT_BLACK,
    uint8_t size = 2,
    uint16_t width = 140
);