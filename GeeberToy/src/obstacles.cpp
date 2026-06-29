#include "obstacles.h"
#include "graphics.h"
#include "sprite_effects.h"
#include "sprite_functions.h"

// If you add the black outline:
// old heights: 6, 12, 23, 33
// new heights: 7, 13, 24, 34
// width: 32 -> 34
static const ResistorVariant variants[OBS_TYPE_COUNT] = {
  {34,  7, 3},  // RES_SHORT
  {34, 13, 3},  // RES_MEDIUM
  {34, 24, 4},  // RES_TALL
  {34, 34, 4},  // RES_HUGE
};

static const CapVariant vcariants[OBS_TYPE_COUNT] = {
  {34,  7, 3},  // RES_SHORT
  {34, 13, 3},  // RES_MEDIUM
  {34, 24, 4},  // RES_TALL
  {34, 34, 4},  // RES_HUGE
};

constexpr int NUM_RESISTOR_VARIANTS = 4;
static SpriteSheet resistorSheet = {ResistorPack_Sheet, 64, 64, NUM_RESISTOR_VARIANTS, 256};

constexpr int CELL_W = 64;
constexpr int CELL_H = 64;

// If resistor is centered in the 64x64 Aseprite frame:
constexpr int RESISTOR_CENTER_OFFSET_X = (CELL_W - 34) / 2;

void obstacleInit(Obstacle &obs) {
  obstacleReset(obs);
}

void obstacleReset(Obstacle &obs) {
  obs.type = (ObstacleType)random(0, NUM_RESISTOR_VARIANTS);

  const ResistorVariant &v = variants[obs.type];

  obs.frame = (int)obs.type;
  obs.speed = v.speed;

  obs.x = SCREEN_W + random(20, 80);
  obs.y = GROUND_Y - CELL_H;

  obs.hitOffsetX = RESISTOR_CENTER_OFFSET_X;
  obs.hitOffsetY = CELL_H - v.hitH;
  obs.hitW = v.hitW;
  obs.hitH = v.hitH;
}

void obstacleUpdate(Obstacle &obs) {
  obs.x -= obs.speed;

  if (obs.x < -CELL_W) {
    obstacleReset(obs);
  }
}

void obstacleDraw(Obstacle &obs, const SpriteSheet &resistorSheet) {
  drawSpriteFrame(
    screen,
    frameSprite,
    resistorSheet,
    obs.frame,
    obs.x,
    obs.y,
    TRANSPARENT
  );

  // Debug hitbox. Comment out once tuned.
  screen.drawRect(
    obs.x + obs.hitOffsetX,
    obs.y + obs.hitOffsetY,
    obs.hitW,
    obs.hitH,
    TFT_RED
  );
}

bool obstacleCollides(const Obstacle &obs, const Player &p) {
  int playerHitX = p.x + 18;
  int playerHitY = (int)p.y + 12;
  int playerHitW = 28;
  int playerHitH = 48;

  int obsHitX = obs.x + obs.hitOffsetX;
  int obsHitY = obs.y + obs.hitOffsetY;

  return playerHitX < obsHitX + obs.hitW &&
         playerHitX + playerHitW > obsHitX &&
         playerHitY < obsHitY + obs.hitH &&
         playerHitY + playerHitH > obsHitY;
}