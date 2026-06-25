#include <Arduino.h>
#include "graphics.h"
#include "sprite_functions.h"
#include "sprites.h"
#include "player.h"
#include "sprite_effects.h"

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

SpriteSheet resistorSheet {
  ResistorPack_Sheet,
  32,
  32,
  4,
  128
};

enum AppState {
  STATE_IDLE,
  STATE_SPEAK,
  STATE_GAME,
  STATE_SLEEP
};

AppState state = STATE_IDLE;

Player player;

struct DebouncedButton { int pin; bool lastStableState; bool lastReading; unsigned long lastChangeTime; };
bool buttonPressed(DebouncedButton &btn);

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

constexpr unsigned long DEBOUNCE_MS = 30;

bool game_start();

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(D5, INPUT_PULLUP);
  pinMode(D6, INPUT_PULLUP);

  graphicsInit();
  playerInit(player);

  state = STATE_GAME;

  Serial.println("BOOT OK");
}

void loop() {
  screen.fillSprite(TFT_WHITE);
  screen.drawLine(0,121,240,121,TFT_BLACK);

  if (buttonPressed(rightBtn) && game_start()) {
    playerStartJump(player);
  }

  playerUpdate(player, boyWalk);

  beginFrame(TFT_WHITE);
  if (player.onGround) {
    drawSpriteFrame(screen, frameSprite, boyWalk, player.idleFrame, player.x, (int)player.y, TRANSPARENT);
  } else {
    drawSpriteFrame(screen, frameSprite, boyJump, playerGetJumpFrame(player), player.x, (int)player.y, TRANSPARENT);
  }
  endFrame();
}

bool game_start()
{
  return state == STATE_GAME;
}

bool buttonPressed(DebouncedButton &btn)
{
  bool reading = digitalRead(btn.pin);

  if (reading != btn.lastReading)
  {
    btn.lastChangeTime = millis();
    btn.lastReading = reading;
  }
  if ((millis() - btn.lastChangeTime) > DEBOUNCE_MS)
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
