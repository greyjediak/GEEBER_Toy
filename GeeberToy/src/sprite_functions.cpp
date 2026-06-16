#include "sprite_functions.h"
#include "sprites.h"
#include "graphics.h"

void eraseSpriteArea(
  Adafruit_ST7789 &disp,
  int x,
  int y,
  int w,
  int h,
  int scale,
  uint16_t bgColor
) {
  disp.fillRect(x, y, w * scale, h * scale, bgColor);
} 

void sprite_walk_right(Adafruit_ST7789 &disp, int xstart, int ystart, int xEnd, int yEnd, int speed)
{
    const uint16_t *frames[] = {
        boy_walk_right_frame1,
        boy_walk_right_frame2,
        boy_walk_right_frame3,
        boy_walk_right_frame4,
    };
    int frame = 0;
    for (int i = xstart; i < xEnd; i +=speed)
    {
        
        drawSpriteTransparent(disp, i, ystart, frames[frame], BOY_W, BOY_H, TRANSPARENT);
        frame=(frame+1)%4;
        eraseSpriteArea(disp, i, ystart, BOY_W, BOY_H/2, 2, ST77XX_WHITE);
    }
};