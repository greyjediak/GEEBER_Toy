// Handles the idle state. Randomly triggers wisdom quotes on a timer.

#include "idle.h"
#include "graphics.h"
#include "sprite_functions.h"
#include "sprites.h"
#include "jokes.h"
#include "time.h"

// SpriteSheet struct: {spritearray from sprites.h, sprite width, sprite height, frame count, sheet width }

static SpriteSheet boyIdle  = { boy_side_idle, 64, 64, 16, 1024 };
static SpriteSheet boySpeak = { boy_speak, 64, 64, 16, 1024 };
static SpriteSheet boyWalk = { boy_walk, 64, 64, 16, 1024};

static int  idleFrame  = 0;
static int  speakFrame = 0;
static unsigned long lastIdleFrameTime  = 0;
static unsigned long lastSpeakFrameTime = 0;
constexpr unsigned long IDLE_FRAME_DELAY  = 90;
constexpr unsigned long SPEAK_FRAME_DELAY = 120;

enum IdleSubState { SUB_IDLE, SUB_SPEAKING };
static IdleSubState  subState     = SUB_IDLE;
static const char   *currentText  = nullptr;
static unsigned long speakEndTime = 0;
static unsigned long nextSpeakTime = 0;

constexpr unsigned long SPEAK_DURATION = 4000; // ms to hold text on screen
constexpr unsigned long SPEAK_MIN_WAIT = 6000; // shortest gap between quips
constexpr unsigned long SPEAK_MAX_WAIT = 1200000;

static void scheduleNextSpeak() {
    nextSpeakTime = millis() + SPEAK_MIN_WAIT + random(SPEAK_MAX_WAIT - SPEAK_MIN_WAIT);
}

void offer_wisdom() {
    currentText        = wisdom[random(NUM_WISDOM)];
    speakFrame         = 0;
    lastSpeakFrameTime = millis();
    speakEndTime       = millis() + SPEAK_DURATION;
    subState           = SUB_SPEAKING;
}

void idle() {
    unsigned long now = millis();

    if (nextSpeakTime == 0) scheduleNextSpeak(); // first call init

    if (subState == SUB_IDLE) {
        if (now - lastIdleFrameTime >= IDLE_FRAME_DELAY) {
            idleFrame = (idleFrame + 1) % boyIdle.frameCount;
            lastIdleFrameTime = now;
        }
        if (now >= nextSpeakTime) offer_wisdom();

        beginFrame(TFT_WHITE);
        drawSpriteFrame(screen, frameSprite, boyIdle, idleFrame, 88, 88, TRANSPARENT);
        endFrame();

    } else { // SUB_SPEAKING
        if (now - lastSpeakFrameTime >= SPEAK_FRAME_DELAY) {
            speakFrame = (speakFrame + 1) % boySpeak.frameCount;
            lastSpeakFrameTime = now;
        }
        if (now >= speakEndTime) {
            subState = SUB_IDLE;
            scheduleNextSpeak();
        }

        beginFrame(TFT_WHITE);
        drawSpriteFrame(screen, frameSprite, boySpeak, speakFrame, 88, 88, TRANSPARENT);
        drawText(currentText, 10, 170, TFT_BLACK, 1, 220);
        endFrame();
    }
}
