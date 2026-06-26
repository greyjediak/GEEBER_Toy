#include <Arduino.h>
#include "graphics.h"
#include "sprite_functions.h"
#include "sprites.h"
#include "player.h"
#include "sprite_effects.h"
#include "idle.h"

SpriteSheet boyIdle = {
  boy_side_idle,
  64,
  64,
  16,
  1024
};

SpriteSheet boyJump = {
  boy_jump,
  64,
  64,
  4,
  256
};

SpriteSheet boyWalk = {
  boy_walk,
  64,
  64,
  4,
  256
};

SpriteSheet resistorSheet = {
  ResistorPack_Sheet,
  32,
  32,
  4,
  128
};

enum AppState {
  STATE_STARTUP,
  STATE_MENU,
  STATE_IDLE,
  STATE_GAME,
  STATE_SLEEP
};

AppState state = STATE_STARTUP;
int menuChoice = 0;

Player player;

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
  playerInit(player);

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
          case 1: playerInit(player); state = STATE_GAME;    break;
          case 2:                     state = STATE_SLEEP;   break;
          case 3:                     state = STATE_STARTUP; break;
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
      if (leftPressed)  state = STATE_MENU;
      if (rightPressed) playerStartJump(player);
      playerUpdate(player, boyWalk);
      beginFrame(TFT_WHITE);
      screen.drawLine(0, 121, 240, 121, TFT_BLACK);
      if (player.onGround) {
        drawSpriteFrame(screen, frameSprite, boyWalk, player.idleFrame, player.x, (int)player.y, TRANSPARENT);
      } else {
        drawSpriteFrame(screen, frameSprite, boyJump, playerGetJumpFrame(player), player.x, (int)player.y, TRANSPARENT);
      }
      endFrame();
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
