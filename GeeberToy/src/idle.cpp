// Handles the idle state for the sprite. Idle state has several other randomly seeeded states.

#include "idle.h"
#include "graphics.h"
#include "sprite_functions.h"
#include "sprites.h"

static SpriteSheet boyIdle = { boy_side_idle, 64, 64, 16, 1024 };

static int idleFrame = 0;
static unsigned long lastIdleTime = 0;
constexpr unsigned long IDLE_FRAME_DELAY = 90;

void idle()
{
    unsigned long now = millis();
    if (now - lastIdleTime >= IDLE_FRAME_DELAY) {
        idleFrame = (idleFrame + 1) % boyIdle.frameCount;
        lastIdleTime = now;
    }
    beginFrame(TFT_WHITE);
    drawSpriteFrame(screen, frameSprite, boyIdle, idleFrame, 88, 88, TRANSPARENT);
    endFrame();
}
