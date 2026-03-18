//
// Created by Fev1L on 18.03.2026.
//

#include "entities.h"

void updateEcho(Game* game, float deltaTime) {
    Echo& e = game->echo;

    if (!game->system.echoSystem) {
        if (!e.active) {
            e.timer = 0.0f;
        }
    } else {
        e.timer += deltaTime;

        if (!e.active && e.timer >= game->cfg.echoInterval) {
            e.active = true;
            e.radius = 0.0f;
            e.timer = 0.0f;
            float r = static_cast<float>(rand()) / RAND_MAX;
            if (r < game->cfg.systemBreakChance) game->system.echoSystem = false;
        }
    }

    if (e.active) {
        e.radius += e.speed * deltaTime;

        float maxRadius = std::max(game->GRID_W, game->GRID_H);
        if (e.radius >= maxRadius) {
            e.active = false;
            e.radius = 0.0f;
        }
    }
}