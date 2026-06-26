#include "idle.h"

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
