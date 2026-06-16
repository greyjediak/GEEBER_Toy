#include "graphics.h"

#define TRANSPARENT 0xF81F

void drawSpriteTransparent(
  Adafruit_ST7789 &disp,
  int x,
  int y,
  const uint16_t *sprite,
  int w,
  int h,
  uint16_t transparent)
{
  for (int py = 0; py < h; py++) {
    int px = 0;

    while (px < w) {
      uint16_t color = sprite[py * w + px];

      if (color == transparent) {
        px++;
        continue;
      }

      int runStart = px;
      int runLength = 1;

      while (
        px + runLength < w &&
        sprite[py * w + px + runLength] == color
      ) {
        runLength++;
      }

      disp.fillRect(x + runStart, y + py, runLength, 1, color);
      px += runLength;
    }
  }
}