#pragma once
#include "sprite_functions.h"
#include "game.h"

enum ObstacleType { RES_SHORT, RES_MEDIUM, RES_TALL, RES_WIDE, CAP_1, CAP_2, CAP_3, CAP_4, OBS_TYPE_COUNT };

struct Obstacle {
    int x;
    int y;
    int frame;
    int hitOffsetX;
    int hitOffsetY;
    int hitW;
    int hitH;
    int speed;
    ObstacleType type;
};

struct ResistorVariant {
    int hitW;
    int hitH;
    int speed;
};

struct CapVariant {
    int hitW;
    int hitH;
    int speed;
};

void obstacleInit(Obstacle &obs);
void obstacleReset(Obstacle &obs);
void obstacleUpdate(Obstacle &obs);
void obstacleDraw(Obstacle &obs, const SpriteSheet &resistorSheet);
bool obstacleCollides(const Obstacle &obs, const Player &p);
