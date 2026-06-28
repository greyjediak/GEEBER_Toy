#include <Arduino.h>
#include "graphics.h"
#include "idle.h"
#include "game.h"
#include "sprites.h"
#include "sprite_functions.h"

enum AppState {
  STATE_STARTUP,
  STATE_MENU,
  STATE_IDLE,
  STATE_GAME,
  STATE_SLEEP,
  STATE_JOKE,
};

static SpriteSheet boyWalk = { boy_speak, 64, 64, 4, 256 };

AppState state = STATE_STARTUP;
int menuChoice = 0;

struct DebouncedButton { int pin; bool lastStableState; bool lastReading; unsigned long lastChangeTime; };
bool buttonPressed(DebouncedButton &btn);

DebouncedButton leftBtn  = { D5, HIGH, HIGH, 0 };
DebouncedButton rightBtn = { D6, HIGH, HIGH, 0 };

constexpr unsigned long DEBOUNCE_MS = 30;
constexpr int NUM_MENU_ITEMS = 4;

void display_menu(int highlighted);

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);

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
      drawCenteredText("Press L: menu", 90);
      drawCenteredText("Press R: idle", 120);
      endFrame();
      break;

    case STATE_MENU:
      if (leftPressed)  menuChoice = (menuChoice + 1) % NUM_MENU_ITEMS;
      if (rightPressed) {
        switch (menuChoice) {
          case 0: /* tell joke — not yet implemented */ break;
          case 1: gameInit(); state = STATE_GAME;    break;
          case 2:             state = STATE_SLEEP;   break;
          case 3:             state = STATE_STARTUP; break;
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
    "4. Quit menu"
  };
  int y = 40;
  for (int i = 0; i < NUM_MENU_ITEMS; i++) {
    uint16_t color = (i == highlighted) ? TFT_BLUE : TFT_BLACK;
    drawText(items[i], 20, y, color, 2, 200);
    y += 50;
  }
}
