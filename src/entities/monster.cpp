//
// Created by Fev1L on 18.03.2026.
//

#include "entities.h"
#include "core/core.h"

void spawnMonster(Game* game){
    Monster m;
    int w = game->GRID_W;
    int h = game->GRID_H;

    do {
        if (rand() % 2) {
            m.x = rand() % w;
            m.y = (rand() % 2) ? 0 : h - 1;
        } else {
            m.y = rand() % h;
            m.x = (rand() % 2) ? 0 : w - 1;
        }
    } while (m.x == game->centerX && m.y == game->centerY);

    m.present = true;
    m.monsterLiveTime = 0.0f;
    m.moveTimer = 0.0f;
    m.visible = false;
    m.visibleTime = 0.0f;

    game->monsters.push_back(m);
}
//=================================================================
void updateMonsters(Game* game, float deltaTime) {
    for (auto& m : game->monsters) {
        updateMonster(m, game, deltaTime);
    }

    game->monsters.erase(
        std::remove_if(game->monsters.begin(), game->monsters.end(), [](const Monster& m) { return !m.present; }),
        game->monsters.end()
    );
}
//=================================================================
bool isCellOccupied(int x, int y, const Game* game) {
    for (const Monster& other : game->monsters) {
        if (other.present && other.x == x && other.y == y) {
            return true;
        }
    }
    return false;
}
void updateMonster(Monster& m, Game* game, float deltaTime) {
    if (m.present) {
        m.monsterLiveTime += deltaTime;
        m.moveTimer += deltaTime;

        if (m.moveTimer >= game->cfg.monsterMoveInterval) {
            Move next = chooseMoveProb(m, game);
            if (!isCellOccupied(next.x, next.y, game)) {
                m.x = next.x;
                m.y = next.y;
            }
            m.moveTimer = 0.0f;
        }

        if (m.monsterLiveTime >= 60.0f) {
            m.present = false;
            m.monsterSpawnTime = rand() % 10;
            m.visible = false;
        }
    }

    else {
        if (m.monsterSpawnTime > 0.0f) {
            m.monsterSpawnTime -= deltaTime;
        } else {
            spawnMonster(game);
        }
    }
}