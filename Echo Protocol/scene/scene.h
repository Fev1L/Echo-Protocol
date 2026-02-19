//
//  scene.h
//  Echo Protocol
//
//  Created by Fev1L on 16.02.2026.
//

#pragma once
#include "../types.h"
#include "../render/render.h"
#include "../logic/logic.h"
#include "../gameUpdates/update.h"

void renderMenu(Game* game, App* app);

void renderGame(Game* game, App* app);

void renderCustomgame(Game* game, App* app);

void renderEndgame(Game* game, App* app);
