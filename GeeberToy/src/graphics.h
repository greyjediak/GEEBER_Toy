#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>

constexpr int SCREEN_W = 240;
constexpr int SCREEN_H = 240;
constexpr uint16_t TRANSPARENT = 0xF81F;

extern TFT_eSPI tft;
extern TFT_eSprite screen;
extern TFT_eSprite frameSprite;

void graphicsInit();
void beginFrame(uint16_t bgColor);
void endFrame();
