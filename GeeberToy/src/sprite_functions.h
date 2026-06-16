#pragma once
#include <Arduino.h>

#include <Adafruit_ST7789.h>

void eraseSpriteArea(
  Adafruit_ST7789 &disp,
  int x,
  int y,
  int w,
  int h,
  int scale,
  uint16_t bgColor
);

void sprite_walk_right(Adafruit_ST7789 &disp, int xstart, int ystart, int xEnd, int yEnd, int speed);