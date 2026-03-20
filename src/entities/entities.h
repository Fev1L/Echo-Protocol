//
// Created by Fev1L on 18.03.2026.
//

#pragma once
#include "../types.h"

// MONSTER

void spawnMonster(Game* game);

void updateMonsters(Game* game, float deltaTime);

void updateMonster(Monster& m, Game* game, float deltaTime);

// BAIT

void spawnNoise(Game* game, int gridX, int gridY);

void updateNoises(Game* game, float deltaTime);

// ECHO

void updateEcho(Game* game, float deltaTime);

// CAMERA

void updateCamera(Game* game, float deltaTime);

// LAMP

void updateTopLamp(Game* game, float deltaTime);