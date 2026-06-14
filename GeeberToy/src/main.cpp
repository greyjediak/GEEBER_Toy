#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "sprites.h"

#include "jokes.h"

/* Display */
#define DISP_CS    D0
#define DISP_RES   D1
#define DISP_DC    D2

#define DISP_SDA   D10   // MOSI
#define DISP_SCL   D8    // SCK

#define TRANSPARENT 0xF81F

Adafruit_ST7789 disp = Adafruit_ST7789(
  DISP_CS,
  DISP_DC,
  DISP_SDA,
  DISP_SCL,
  DISP_RES
);



void drawSpriteTransparent(int x, int y, const uint16_t *sprite, int w, int h, uint16_t transparent);
void tellRandomJoke();
struct DebouncedButton { int pin; bool lastStableState; bool lastReading; unsigned long lastChangeTime;};
bool buttonPressed(DebouncedButton &btn);

/* Buttons */
DebouncedButton leftBtn =
{
    D5,
    HIGH,
    HIGH,
    0
};

DebouncedButton rightBtn =
{
    D6,
    HIGH,
    HIGH,
    0
};
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);

  Serial.println("Init display");
  disp.init(240, 240);
  disp.setRotation(0);
  disp.fillScreen(ST77XX_WHITE);
  Serial.println("Screen should be white");
}

void loop() {

  // Button + blink test
  if (buttonPressed(leftBtn))
  {
    Serial.println("Left Pressed");
    disp.fillScreen(ST77XX_WHITE);
    drawSpriteTransparent(104,104,boy_front_idle,BOY_W,BOY_H,TRANSPARENT);
  }
  if (buttonPressed(rightBtn))
  {
    Serial.println("right Pressed");
    disp.fillScreen(ST77XX_WHITE);
    drawSpriteTransparent(104, 50, boy_front_eyes_closed,BOY_W,BOY_H,TRANSPARENT);
  }

}

bool buttonPressed(DebouncedButton &btn)
{
  const unsigned long debounceMs = 30; // 30 ms debounce delay
  bool reading = digitalRead(btn.pin);

  if (reading != btn.lastReading)
  {
    btn.lastChangeTime = millis();
    btn.lastReading = reading;
  }
  if ((millis() - btn.lastChangeTime) > debounceMs)
  {
    if (reading != btn.lastStableState)
    {
      btn.lastStableState = reading; 

      if (reading == LOW)
      {
        return true;
      }
    }
  }
  return false;
}

void drawSpriteTransparent(
    int x,
    int y,
    const uint16_t *sprite,
    int w,
    int h,
    uint16_t transparent)
{
    for(int py = 0; py < h; py++)
    {
        for(int px = 0; px < w; px++)
        {
            uint16_t color = sprite[py * w + px];

            if(color == transparent)
                continue;

            disp.drawPixel(x + px, y + py, color);
        }
    }
}

void tellRandomJoke() // Tell random joke, TODO, connect to accelerometer
{
  int index = random(NUM_JOKES);
  disp.println(jokes[index].setup);
  delay (2000);
  disp.println(jokes[index].punchline);
}