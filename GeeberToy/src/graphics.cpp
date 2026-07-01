//Handles only screen init, off screen drawing, and text wrapping.

#include "graphics.h"
#include <time.h>
#include <sys/time.h>
#include "backgrounds.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite screen = TFT_eSprite(&tft);
TFT_eSprite frameSprite = TFT_eSprite(&tft);

void graphicsInit() {
  tft.init();
  tft.setRotation(2);
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
  drawClock();
  screen.pushSprite(0, 0);
}

void drawJumpBackground()
{
  screen.setSwapBytes(true);
  screen.pushImage(0, 0, 240, 240, game_background);
  screen.setSwapBytes(false);
}

void setClock(int hour, int minute)
{
  struct tm t = {0};
  t.tm_year = 2026 - 1900;
  t.tm_mon = 0;
  t.tm_mday = 1;
  t.tm_hour = hour;
  t.tm_min = minute;
  struct timeval tv = { .tv_sec = mktime(&t), .tv_usec = 0};
  settimeofday(&tv, nullptr);
}

void drawClock() 
{
  time_t now = time(nullptr);
  struct tm *lt = localtime(&now);
  char buf[6];
  strftime(buf, sizeof(buf), "%H:%M", lt);

  const int pad = 3;
  screen.setTextSize(1);
  screen.setTextDatum(TR_DATUM);
  screen.setTextColor(TFT_WHITE, TFT_BLACK);

  int w = screen. textWidth(buf) + pad * 2;
  int h = 8 + pad *2;
  screen.fillRoundRect(SCREEN_W - w, 0, w, h, 3, TFT_BLACK);
  screen.drawString(buf, SCREEN_W - pad, pad);
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

