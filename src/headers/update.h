//
//  update.h
//  Echo Protocol
//
//  Created by Fev1L on 19.02.2026.
//

#pragma once
#include "types.h"
#include "logic.h"

void updateMonsters(Game* game, float deltaTime);

void updateMonster(Monster& m,const Game* game, float deltaTime);

void updateNoises(Game* game, float deltaTime);

void updateNoise(Noise& n, Game* game, float dt);

void updateGameClock(Game* game, float deltaTime);

void updateEcho(Game* game, float deltaTime);

void updateCamera(Game* game, float deltaTime);

void updateRepair(Game* game, float dt);

