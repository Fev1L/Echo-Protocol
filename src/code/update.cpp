//
//  update.cpp
//  Echo Protocol
//
//  Created by Fev1L on 19.02.2026.
//

#include "update.h"

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
            spawnMonster(const_cast<Game*>(game));
        }
    }
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
//=================================================================
void updateGameClock(Game* game, float deltaTime) {
    game->gameTime += deltaTime;

    float gameMinutesPassed = (game->gameTime / game->cfg.REAL_SECONDS_PER_15_MIN) * 15.0f;
    int totalMinutes = static_cast<int>(gameMinutesPassed);

    game->hours   = totalMinutes / 60;
    if(totalMinutes % 15 == 0)
    game->minutes = totalMinutes % 60;
}
//=================================================================
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
//=================================================================
void updateCamera(Game* game, float deltaTime) {
    float speed = 360.0f;

    float diff = game->viewAngleTarget - game->viewAngle;
    float step = speed * deltaTime;

    if (fabs(diff) <= step) {
        game->viewAngle = game->viewAngleTarget;
        game->currentView = game->targetView;
        game->camera.isTurning = false;
    } else {
        game->viewAngle += (diff > 0 ? step : -step);
        game->camera.isTurning = true;
    }

    if(game->viewAngle == 0)
        game->currentView = ViewSide::CENTER;
}
//=================================================================
void updateRepair(Game* game, float dt){
    if (!game->system.active)
        return;

    game->system.timer += dt;

    if (game->system.timer >= game->system.duration){
        game->system.active = false;

        switch (game->system.type)
        {
            case RepairType::BAIT:
                game->system.baitSystem = true;
                break;

            case RepairType::ECHO:
                game->system.echoSystem = true;
                break;

            case RepairType::TRACK:
                game->system.trackingSystem = true;
                break;
                
            case RepairType::REBOOT:
                game->system.baitSystem = true;
                game->system.echoSystem = true;
                game->system.trackingSystem = true;
                break;

            default:
                break;
        }
        game->system.type = RepairType::NONE;
    }
}
