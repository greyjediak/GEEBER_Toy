#include "obstacles.h"
#include "graphics.h"
#include "sprite_functions.h"
#include "sprite_effects.h"

// Sheet is 256x64: OBS_TYPE_COUNT frames of 64x64, one per resistor variant.
// All resistors must be BOTTOM-ALIGNED in Aseprite so the draw position
// stays constant (groundY - CELL_H) regardless of variant height.
constexpr int CELL_H   = 64;
constexpr int CELL_W   = 64;

static SpriteSheet resistorSheet = {
    ResistorPack_Sheet,
    CELL_W, CELL_H, OBS_TYPE_COUNT, 256
};

static TFT_eSprite obstacleSprite(&tft);

// Per-variant collision box. visW and visH are the tight pixel bounds of
// the actual resistor art (not the full 64x64 cell).
// Fill these in from your Aseprite measurements.
struct ResistorVariant {
    int   visW;
    int   visH;
    int   speed;
    float jumpPower; // negative — more negative = higher jump
};

// jumpPower: tune these so each variant is just barely clearable with a well-timed press.
// More negative = higher jump needed.
static const ResistorVariant variants[OBS_TYPE_COUNT] = {
    { 28, 18, 3, -6.5f  },  // RES_SHORT  — low hop
    { 28, 32, 3, -9.5f  },  // RES_MEDIUM — standard jump
    { 28, 48, 4, -13.0f },  // RES_TALL   — big jump, press early
    { 48, 28, 4, -9.5f  },  // RES_WIDE   — same height, press early for gap
};

void obstaclesInit() {
    static bool created = false;
    if (created) return;
    obstacleSprite.setColorDepth(16);
    obstacleSprite.createSprite(CELL_W, CELL_H);
    obstacleSprite.setSwapBytes(true);
    created = true;
}

void obstacleSpawn(Obstacle &obs) {
    ObstacleType t = (ObstacleType)random(OBS_TYPE_COUNT);
    const ResistorVariant &v = variants[t];
    obs.x     = 240;
    obs.y     = GROUND_Y - v.visH; // collision top, bottom sits on ground
    obs.w     = v.visW;
    obs.h     = v.visH;
    obs.speed = v.speed;
    obs.type  = t;
}

void obstacleUpdate(Obstacle &obs) {
    obs.x -= obs.speed;
    if (obs.x + obs.w < 0) obstacleSpawn(obs);
}

void obstacleDraw(Obstacle &obs) {
    // Draw Y is always the top of the cell. Since all resistors are
    // bottom-aligned in the cell, this places every variant on the ground.
    int drawY = GROUND_Y - CELL_H;
    drawSpriteFrame(screen, obstacleSprite, resistorSheet, (int)obs.type, obs.x, drawY, TRANSPARENT);
}

float obstacleJumpPower(const Obstacle &obs) {
    return variants[obs.type].jumpPower;
}

bool obstacleCollides(const Obstacle &obs, const Player &p) {
  int playerHitX = p.x + 18;
  int playerHitY = (int)p.y + 12;
  int playerHitW = 28;
  int playerHitH = 48;

  return playerHitX < obs.x + obs.w &&
         playerHitX + playerHitW > obs.x &&
         playerHitY < obs.y + obs.h &&
         playerHitY + playerHitH > obs.y;
}
