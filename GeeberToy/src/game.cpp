#include "game.h"
#include "graphics.h"
#include "sprite_functions.h"
#include "sprites.h"
#include "sprite_effects.h"
#include "obstacles.h"

static SpriteSheet boyWalk = { boy_walk, 64, 64, 4, 256 };
static SpriteSheet boyJump = { boy_jump, 64, 64, 4, 256 };
static SpriteSheet resistorSheet = {ResistorPack_Sheet, 64, 64, 4, 256};


constexpr float GRAVITY = 0.60;
constexpr float JUMP_POWER = -9.5;
constexpr unsigned long IDLE_FRAME_DELAY = 90;

static Player player;
static Obstacle obstacle;

void gameInit() {
    playerInit(player);
    obstacleInit(obstacle);
}

void playerInit(Player &p) {
  p.x = 88;
  p.y = GROUND_Y - PLAYER_H; // Ground is at 152 right now
  p.velocityY = 0;
  p.onGround = true;
  p.idleFrame = 0;
  p.lastIdleAnimTime = 0;
}

bool gameUpdate(bool jumpPressed, bool exitPressed) {
    if (exitPressed) return true;
    if (jumpPressed) playerStartJump(player);
    playerUpdate(player, boyWalk);
    obstacleUpdate(obstacle);
    if (obstacleCollides(obstacle, player)) {
        return true;
    }
    return false;
}

void gameDraw() {
    beginFrame(TFT_WHITE);
    drawJumpBackground();
    screen.drawLine(0, GROUND_Y, SCREEN_W, GROUND_Y, TFT_GREEN);

    obstacleDraw(obstacle, resistorSheet);

    if (player.onGround) {
        drawSpriteFrame(screen, frameSprite, boyWalk, player.idleFrame, player.x, (int)player.y, TRANSPARENT);
    } else {
        drawSpriteFrame(screen, frameSprite, boyJump, playerGetJumpFrame(player), player.x, (int)player.y, TRANSPARENT);
    }
    endFrame();
}

// Player

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
      p.y = GROUND_Y - PLAYER_H;
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
