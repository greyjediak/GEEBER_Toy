#pragma once

#include <Arduino.h>
#include <Adafruit_ST7789.h>

#define TRANSPARENT 0xF81F

void drawSpriteTransparent(
  Adafruit_ST7789 &disp,
  int x,
  int y,
  const uint16_t *sprite,
  int w,
  int h,
  uint16_t transparent
);