#pragma once
#include "player.h"

// One entry per column in the resistor sheet.
// Add/rename these to match what you drew.
enum ObstacleType { RES_SHORT, RES_MEDIUM, RES_TALL, RES_WIDE, OBS_TYPE_COUNT };

struct Obstacle {
    int x;
    int y;           // collision top  (groundY - visH, set by obstacleInit)
    int w;           // collision width
    int h;           // collision height
    int speed;
    ObstacleType type;
};

void  obstaclesInit();
void  obstacleSpawn(Obstacle &obs);
void  obstacleUpdate(Obstacle &obs);
void  obstacleDraw(Obstacle &obs);
bool  obstacleCollides(const Obstacle &obs, const Player &p);
float obstacleJumpPower(const Obstacle &obs); // required jump power to clear this obstacle
