#include "player.h"

constexpr int GROUND_Y = 120;
constexpr float GRAVITY = 0.55;
constexpr float JUMP_POWER = -9.5;
constexpr unsigned long IDLE_FRAME_DELAY = 90;

void playerInit(Player &p) {
  p.x = 88;
  p.y = GROUND_Y;
  p.velocityY = 0;
  p.onGround = true;
  p.idleFrame = 0;
  p.lastIdleAnimTime = 0;
}

void playerStartJump(Player &p) {
  if (p.onGround) {
    p.velocityY = JUMP_POWER;
    p.onGround = false;
  }
}

void playerUpdate(Player &p, const SpriteSheet &idleSheet) {
  if (!p.onGround) {
    p.velocityY += GRAVITY;
    p.y += p.velocityY;

    if (p.y >= GROUND_Y) {
      p.y = GROUND_Y;
      p.velocityY = 0;
      p.onGround = true;
    }
  }

  unsigned long now = millis();
  if (now - p.lastIdleAnimTime >= IDLE_FRAME_DELAY) {
    p.idleFrame = (p.idleFrame + 1) % idleSheet.frameCount;
    p.lastIdleAnimTime = now;
  }
}

int playerGetJumpFrame(const Player &p) {
  if (p.velocityY < 0) return 1;
  return 2;
}