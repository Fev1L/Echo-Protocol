//
//  logic.h
//  Echo Protocol
//
//  Created by Fev1L on 17.01.2026.
//

#pragma once
#include "../types.h"
#include "render.h"

void spawnMonster(Game* game);

bool inBounds(int x, int y, int GRID_W, int GRID_H);

std::vector<Move> getMoves(const Monster& m);

inline int distance(int x1, int y1, int x2, int y2);

void spawnNoise(Game* game, int gridX, int gridY);

void getTarget(const Game* game, int monsterX, int monsterY, int& tx, int& ty);

Move chooseMoveProb(const Monster& m,const Game* game);

void updateMonster(Monster& m,const Game* game, float deltaTime);

void updateNoise(Game* game, float dt);

void updateGameClock(Game* game, float deltaTime);

void updateEcho(Game* game, float deltaTime);

void checkEchoHit(Game* game, float deltaTime);

void updateCamera(Game* game, float deltaTime);
