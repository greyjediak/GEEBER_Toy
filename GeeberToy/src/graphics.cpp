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

int drawText(
    const String &text,
    int x,
    int y,
    uint16_t color,
    uint8_t size,
    uint16_t maxWidth)
{
  screen.setTextColor(color, TFT_WHITE);
  screen.setTextSize(size);
  screen.setTextDatum(TL_DATUM);

  int lineHeight = 8 * size + 2; //default

  String line = "";
  int lineY = y;

  int start = 0;
  while(start < text.length())
  {
    int end = text.indexOf(' ', start);

    if (end == -1) end = text.length();
    String word = text.substring(start, end);

    String testLine;
    if (line.length() == 0)
    {
      testLine = word;
    }
    else{
      testLine = line + " " + word;
    }
    if (screen.textWidth(testLine) <= maxWidth)
    {
      line = testLine;
    }
    else
    {
      screen.drawString(line, x, lineY);
      lineY += lineHeight;
      line = word;
    }
    start = end + 1;
  }

  if (line.length() > 0)
  {
    screen.drawString(line, x, lineY);
  }

  return lineY + lineHeight - y;

};

void drawCenteredText(
    const String &text,
    int y,
    uint16_t color,
    uint8_t size,
    uint16_t width)
{
  screen.setTextColor (color, TFT_WHITE);
  screen.setTextSize(size);
  screen.setTextDatum(TC_DATUM);

  screen.drawString(text, SCREEN_W / 2, y);
};

