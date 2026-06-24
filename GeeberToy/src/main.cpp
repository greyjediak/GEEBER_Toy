#include <Arduino.h>
#include "graphics.h"
#include "sprite_functions.h"
#include "sprites.h"

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

enum AppState {
  STATE_IDLE,
  STATE_SPEAK,
  STATE_GAME,
  STATE_SLEEP
};

/* ================================== */
/*        STATE FUNCTIONALITY         */
/* ================================== */
AppState state = STATE_IDLE;
unsigned long stateStartTime = 0;

/* ===================================*/
/*            JUMP PARAMS             */
/* ===================================*/
int PLAYER_X = 88;
int playerY = 120;
constexpr int GROUND_Y = 120; //ground is always here

float playerY = GROUND_Y;
float velocityY = 0;

constexpr float GRAVITY = 0.55;
constexpr float JUMP_POWER = -9.5;

bool onGround = true;
/* ===================================*/

/* ================================== */
/*        JUMP FUNCTIONS              */
/* ================================== */
void startJump();
void updateJumpPhysics();
void updateIdleAnimation();
int getJumpFrame();
/* =================================== */

int idleFrame = 0;
unsigned long lastIdleAnimTime = 0;
constexpr unsigned long IDLE_FRAME_DELAY = 90;

// Button Functions
struct DebouncedButton { int pin; bool lastStableState; bool lastReading; unsigned long lastChangeTime;};
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
bool lastButtonState = HIGH;
bool stableButtonState = HIGH;
unsigned long lastButtonChangeTime = 0;
constexpr unsigned long DEBOUNCE_MS = 30;


void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(D5, INPUT_PULLUP); // init left button
  pinMode(D6, INPUT_PULLUP); // init right button

  graphicsInit();

  Serial.println("BOOT OK");
}

void loop() {

  // In the loop, once the device is switched on, we should start at the same point every time.
  // Wake up sprite
  // Greeting
  // main menu with options

  if (buttonPressed(rightBtn) && game_start()) {
    startJump();
  }

  updateJumpPhysics();
  updateIdleAnimation();

  beginFrame(TFT_WHITE);

  if (onGround) {
    drawSpriteFrame(
      screen,
      frameSprite,
      boyIdle,
      idleFrame,
      PLAYER_X,
      (int)playerY,
      TRANSPARENT
    );
  } else {
    drawSpriteFrame(
      screen,
      frameSprite,
      boyJump,
      getJumpFrame(),
      PLAYER_X,
      (int)playerY,
      TRANSPARENT
    );
  }

  endFrame();
}

void startJump() {
  if (onGround) {
    velocityY = JUMP_POWER;
    onGround = false;
  }
}

void updateJumpPhysics() {
  if (!onGround) {
    velocityY += GRAVITY;
    playerY += velocityY;

    if (playerY >= GROUND_Y) {
      playerY = GROUND_Y;
      velocityY = 0;
      onGround = true;
    }
  }
}

void updateIdleAnimation() {
  unsigned long now = millis();

  if (now - lastIdleAnimTime >= IDLE_FRAME_DELAY) {
    idleFrame = (idleFrame + 1) % boyIdle.frameCount;
    lastIdleAnimTime = now;
  }
}

int getJumpFrame() {
  if (velocityY < -3.0) return 0;  // takeoff / rising
  if (velocityY < 0.8)  return 1;  // apex
  if (velocityY < 5.0)  return 2;  // falling / air
  return 3;                        // landing
}

// This only happens when selected from main menu or startup
bool game_start()
{
  if (STATE_GAME)
  {
    return true;
  }
  return false;
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