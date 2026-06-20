#include <Arduino.h>
#include <TFT_eSPI.h>

#include "sprites.h"
#include "jokes.h"

/* Display */
#define DISP_CS    D0
#define DISP_RES   D1
#define DISP_DC    D2

#define DISP_SDA   D10   // MOSI
#define DISP_SCL   D8    // SCK

#define TRANSPARENT 0xF81F

#define SCREEN_W 240
#define SCREEN_H 240

#define FRAME_W 64
#define FRAME_H 64
#define IDLE_FRAMES 32
#define SHEET_W 2048

TFT_eSPI disp = TFT_eSPI();
TFT_eSprite screen = TFT_eSprite(&disp);

int playerX = 88;
int playerY =120;

int idleFrame = 0;
unsigned long lastAnimTime = 0;
const int idleFrameDelay = 90; 

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

void drawFrameFromSheet(
  TFT_eSprite &canvas,
  const uint16_t *sheet,
  int frameIndex,
  int x,
  int y
) {
  int sourceX = frameIndex * FRAME_W;

  for (int row = 0; row < FRAME_H; row++)
  {
    const uint16_t *rowPtr = &sheet[row * SHEET_W + sourceX];

    canvas.pushImage(x, y+row, FRAME_W, 1, rowPtr); // TODO, implement version supporting background color
    
  }
}


void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);

  Serial.println("Init display");
  disp.init();
  disp.setRotation(0);
  disp.fillScreen(TFT_BLACK);
  screen.setColorDepth(16);
  screen.createSprite(SCREEN_W, SCREEN_H);

  Serial.println("Screen should be white");
}

bool played = false;
void loop() {

  unsigned long now  = millis();

  if (now - lastAnimTime >= idleFrameDelay)
  {
    idleFrame = (idleFrame + 1) % IDLE_FRAMES;
    lastAnimTime = now;
  }

  screen.fillSprite(TFT_BLACK);

  drawFrameFromSheet(screen, boy_side_idle, idleFrame, playerX, playerY);
  screen.pushSprite(0, 0);
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

void tellRandomJoke() // Tell random joke, TODO, connect to accelerometer
{
  int index = random(NUM_JOKES);
  disp.println(jokes[index].setup);
  delay (2000);
  disp.println(jokes[index].punchline);
}