//
// Created by Fev1L on 18.03.2026.
//

#include "entities.h"

void spawnNoise(Game* game, int gridX, int gridY) {
    Noise n;

    if (game->noiseCooldown > 0.0f) return;

    n.x = gridX;
    n.y = gridY;
    n.active = true;
    n.timeLeft = 10.0f;

    game->noise.push_back(n);

    game->noiseCooldown = 10.0f * game->cfg.baitReload;
    float r = static_cast<float>(rand()) / RAND_MAX;
    if (r < game->cfg.systemBreakChance) game->system.baitSystem = false;
}
//=================================================================

void updateNoises(Game* game, float deltaTime) {
    if (game->noiseCooldown > 0.0f) game->noiseCooldown -= deltaTime;

    for (auto& n : game->noise) {
        if (n.active) {
            n.timeLeft -= deltaTime;
            if (n.timeLeft <= 0.0f) {
                n.active = false;
            }
        }
    }

    game->noise.erase(
        std::remove_if(game->noise.begin(), game->noise.end(),
            [](const Noise& n){ return !n.active; }),
        game->noise.end()
    );
}