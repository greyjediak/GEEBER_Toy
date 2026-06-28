#include "game.h"
#include "graphics.h"
#include "sprite_functions.h"
#include "sprites.h"
#include "player.h"
#include "obstacles.h"

static SpriteSheet boyWalk = { boy_walk, 64, 64, 4, 256 };
static SpriteSheet boyJump = { boy_jump, 64, 64, 4, 256 };

static Player player;
static Obstacle obstacle;

void gameInit() {
    obstaclesInit();
    playerInit(player);
    obstacleSpawn(obstacle);
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
    screen.drawLine(0, GROUND_Y, SCREEN_W, GROUND_Y, TFT_GREEN);

    obstacleDraw(obstacle);

    if (player.onGround) {
        drawSpriteFrame(screen, frameSprite, boyWalk, player.idleFrame, player.x, (int)player.y, TRANSPARENT);
    } else {
        drawSpriteFrame(screen, frameSprite, boyJump, playerGetJumpFrame(player), player.x, (int)player.y, TRANSPARENT);
    }
    endFrame();
}
