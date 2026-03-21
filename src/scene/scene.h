//
//  scene.h
//  Echo Protocol
//
//  Created by Fev1L on 16.02.2026.
//

#pragma once
#include "../types.h"
#include "core/core.h"
#include "entities/entities.h"

void renderMenu(Game* game, App* app);

void renderGame(Game* game, App* app);

void renderCustomgame(Game* game, App* app);

void renderEndgame(Game* game, App* app);

void renderWinScreen(Game* game, App* app);

void renderLoseScreen(Game* game, App* app);

void renderTutorial(Game* game, App* app);