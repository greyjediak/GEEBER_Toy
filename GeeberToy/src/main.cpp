#include <Arduino.h>
#include "graphics.h"
#include "idle.h"
#include "game.h"
#include "jokes.h"
#include "sprites.h"
#include "sprite_functions.h"
#include "idle.h"

#define TFT_BLK D3 // Moved this from VCC

enum AppState {
  STATE_STARTUP,
  STATE_MENU,
  STATE_IDLE,
  STATE_GAME,
  STATE_SLEEP,
  STATE_JOKE,
  STATE_SETCLOCK
};


AppState state = STATE_STARTUP;
int menuChoice = 0;

static SpriteSheet jokeSpeak = { boy_speak, 64, 64, 16, 1024 };

static int           jokeIdx            = 0;
static bool          jokePunchline      = false;
static int           jokeFrame          = 0;
static unsigned long lastJokeFrameTime  = 0;
static unsigned long jokeSetupStartTime = 0;

constexpr unsigned long JOKE_SETUP_DURATION = 10000; // ms before punchline auto-reveals

static int setHour = 12;
static int setMinute = 0;
static int clockField = 0;

struct DebouncedButton { int pin; bool lastStableState; bool lastReading; unsigned long lastChangeTime; };
bool buttonPressed(DebouncedButton &btn);

DebouncedButton leftBtn  = { D5, HIGH, HIGH, 0 };
DebouncedButton rightBtn = { D6, HIGH, HIGH, 0 };

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr int NUM_MENU_ITEMS = 5;

void display_menu(int highlighted);

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);

  randomSeed(esp_random());
  graphicsInit();
  gameInit();

  screen.setTextColor(TFT_BLACK, TFT_WHITE);
  screen.setTextDatum(TL_DATUM);
  screen.setFreeFont(NULL);
  screen.setTextSize(2);

  Serial.println("BOOT OK");
}

void loop() {
  bool leftPressed  = buttonPressed(leftBtn);
  bool rightPressed = buttonPressed(rightBtn);

  switch (state) {

    case STATE_STARTUP:
      if (leftPressed)       state = STATE_MENU;
      else if (rightPressed) state = STATE_IDLE;
      beginFrame(TFT_WHITE);
      drawText("Menu", 50, 200, TFT_BLACK, 2, 220);
      drawText("Do nothing", 50, 200, TFT_BLACK, 2, 220);
      endFrame();
      break;

    case STATE_MENU:
      if (leftPressed)  menuChoice = (menuChoice + 1) % NUM_MENU_ITEMS;
      if (rightPressed) {
        switch (menuChoice) {
          case 0:
            jokeIdx            = random(NUM_JOKES);
            jokePunchline      = false;
            jokeSetupStartTime = millis();
            state              = STATE_JOKE;
            break;
          case 1: gameInit(); state = STATE_GAME;    break;
          case 2:             state = STATE_SLEEP;   break;
          case 3:             state = STATE_SETCLOCK; break;
          case 4: state = STATE_STARTUP; break;
        }
      }
      beginFrame(TFT_WHITE);
      display_menu(menuChoice);
      endFrame();
      break;

    case STATE_IDLE:
      if (leftPressed) state = STATE_MENU;
      idle();
      break;

    case STATE_GAME:
      if (gameUpdate(rightPressed, leftPressed)) state = STATE_MENU;
      gameDraw();
      break;

    case STATE_SLEEP:
      if (leftPressed || rightPressed) state = STATE_STARTUP;
      beginFrame(TFT_BLACK);
      endFrame();
      break;

    case STATE_JOKE:
      if (millis() - lastJokeFrameTime >= 120) {
        jokeFrame = (jokeFrame + 1) % jokeSpeak.frameCount;
        lastJokeFrameTime = millis();
      }
      if (!jokePunchline) {
        if (rightPressed) jokePunchline = true;                                  // skip early
        if (millis() - jokeSetupStartTime >= JOKE_SETUP_DURATION) jokePunchline = true; // auto after 10s
        if (leftPressed)  state = STATE_MENU;
        beginFrame(TFT_WHITE);
        drawSpriteFrame(screen, frameSprite, jokeSpeak, jokeFrame, 88, 120, TRANSPARENT);
        drawText(jokes[jokeIdx].setup, 10, 60, TFT_BLACK, 1, 220);
        drawCenteredText("R: punchline", 200, TFT_BLUE, 1);
        endFrame();
      } else {
        if (rightPressed || leftPressed) state = STATE_MENU;
        beginFrame(TFT_WHITE);
        drawSpriteFrame(screen, frameSprite, jokeSpeak, jokeFrame, 88, 120, TRANSPARENT);
        drawText(jokes[jokeIdx].punchline, 10, 60, TFT_BLACK, 1, 220);
        drawCenteredText("any: back", 200, TFT_BLUE, 1);
        endFrame();
      }
      break;
    
    case STATE_SETCLOCK:
      if(leftPressed) {
        if (clockField == 0) setHour = (setHour + 1) % 24;
        else setMinute = (setMinute +1) % 60;
      }
      if (rightPressed) 
      {
        if (clockField == 0)
        {
          clockField = 1;
        }
        else
        {
          setClock(setHour, setMinute);
          state = STATE_MENU;
        }
      }
      // Draw clock set
      beginFrame(TFT_WHITE);
      drawCenteredText("Set Clock", 30, TFT_BLACK, 2);
      {
        char buf[6];
        snprintf(buf, sizeof(buf), "%02d:%02d", setHour, setMinute);
        drawCenteredText(buf, 80, TFT_BLUE, 4);
        const char* hint = (clockField == 0) ? "L:+hour R:next" : "L+min R:done";
        drawCenteredText(hint, 170, TFT_BLACK, 1);
      }
      endFrame();
      break;

  }
}

bool buttonPressed(DebouncedButton &btn)
{
  bool reading = digitalRead(btn.pin);
  if (reading != btn.lastReading) {
    btn.lastChangeTime = millis();
    btn.lastReading = reading;
  }
  if ((millis() - btn.lastChangeTime) > DEBOUNCE_MS) {
    if (reading != btn.lastStableState) {
      btn.lastStableState = reading;
      if (reading == LOW) return true;
    }
  }
  return false;
}

void display_menu(int highlighted)
{
  const char* items[NUM_MENU_ITEMS] = {
    "1. Tell joke",
    "2. Play game",
    "3. Sleep",
    "4. Set time",
    "5. Quit menu"
  };
  int y = 30;
  for (int i = 0; i < NUM_MENU_ITEMS; i++) {
    uint16_t color = (i == highlighted) ? TFT_BLUE : TFT_BLACK;
    drawText(items[i], 20, y, color, 2, 200);
    y += 50;
  }
}
