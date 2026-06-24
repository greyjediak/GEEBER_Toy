#include "graphics.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite screen = TFT_eSprite(&tft);
TFT_eSprite frameSprite = TFT_eSprite(&tft);

void graphicsInit() {
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_WHITE);

  screen.setColorDepth(16);
  screen.createSprite(SCREEN_W, SCREEN_H);

  frameSprite.setColorDepth(16);
  frameSprite.createSprite(64, 64);
  frameSprite.setSwapBytes(true);
}

void beginFrame(uint16_t bgColor) {
  screen.fillSprite(bgColor);
}

void endFrame() {
  screen.pushSprite(0, 0);
}