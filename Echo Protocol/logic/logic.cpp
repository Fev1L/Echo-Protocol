//
//  logic.cpp
//  Echo Protocol
//
//  Created by Fev1L on 17.01.2026.
//

#include "logic.h"

//=================================================================
void spawnMonster(Game* game) {
    Monster& m = game->monster;
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
}

//=================================================================
bool inBounds(int x, int y) {
    return x >= 0 && y >= 0 && x < 75 && y < 55;
}
//=================================================================
std::vector<Move> getMoves(const Monster& m) {
    std::vector<Move> moves;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue; // не залишаємося на місці
            int nx = m.x + dx;
            int ny = m.y + dy;
            if (inBounds(nx, ny)) {
                moves.push_back({nx, ny});
            }
        }
    }

    return moves;
}
//=================================================================
inline int distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}
//=================================================================
void spawnNoise(Game* game, int gridX, int gridY) {
    Noise& n = game->noise;

    if (n.cooldown > 0.0f || n.active)
        return;

    n.x = gridX;
    n.y = gridY;
    n.active = true;
    n.timeLeft = 10.0f;
    n.cooldown = 15.0f;
}
//=================================================================
void getTarget(const Game* game, int monsterX, int monsterY, int& tx, int& ty) {
    const Noise& n = game->noise;

    if (n.active) {
        int dx = n.x - monsterX;
        int dy = n.y - monsterY;
        int dist = abs(dx) + abs(dy);

        if (dist <= n.radius) {
            tx = n.x;
            ty = n.y;
            return;
        }
    }

    tx = game->centerX;
    ty = game->centerY;
}
//=================================================================
Move chooseMoveProb(const Monster& m, const Game* game) {
    int tx, ty;
    getTarget(game, m.x, m.y, tx, ty);

    auto moves = getMoves(m);

    if (moves.empty())
        return { m.x, m.y };

    float randomChance = (float)rand() / RAND_MAX;
    if (randomChance < 0.1f) {
        int idx = rand() % moves.size();
        return moves[idx];
    }

    Move bestMove = moves[0];
    int bestDist = distance(moves[0].x, moves[0].y, tx, ty);

    for (auto& mv : moves) {
        int d = distance(mv.x, mv.y, tx, ty);
        if (d < bestDist) {
            bestDist = d;
            bestMove = mv;
        }
    }

    return bestMove;
}
//=================================================================
void updateMonster(Monster& m, const Game* game, float deltaTime) {
    if (m.present) {
        m.monsterLiveTime += deltaTime;
        m.moveTimer += deltaTime;

        if (m.moveTimer >= 1.0f) {
            Move next = chooseMoveProb(m, game);
            m.x = next.x;
            m.y = next.y;
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
void updateNoise(Game* game, float deltaTime) {
    Noise& n = game->noise;

    if (n.cooldown > 0.0f)
        n.cooldown -= deltaTime;

    if (n.active) {
        n.timeLeft -= deltaTime;
        if (n.timeLeft <= 0.0f) {
            n.active = false;
        }
    }
}
//=================================================================
void updateGameClock(Game* game, float deltaTime) {
    game->gameTime += deltaTime;

    float gameMinutesPassed = (game->gameTime / REAL_SECONDS_PER_15_MIN) * 15.0f;

    int totalMinutes = static_cast<int>(gameMinutesPassed);

    game->hours   = totalMinutes / 60;
    if(totalMinutes % 15 == 0)
    game->minutes = totalMinutes % 60;
}
//=================================================================
void updateEcho(Game* game, float deltaTime) {
    Echo& e = game->echo;

    e.timer += deltaTime;

    if (!e.active && e.timer >= e.interval) {
        e.active = true;
        e.radius = 0.0f;
        e.timer = 0.0f;
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
void checkEchoHit(Game* game, float deltaTime) {
    Monster& m = game->monster;
    Echo& e = game->echo;

    if (!e.active || !m.present) return;

    int dx = m.x - game->centerX;
    int dy = m.y - game->centerY;
    float dist = sqrtf(dx*dx + dy*dy);

    if (fabs(dist - e.radius) < 0.5f) {
        m.visible = true;
        m.visibleTime = 2.0f;
    }

    if (m.visible) {
        m.visibleTime -= deltaTime;
        if (m.visibleTime <= 0.0f) {
            m.visible = false;
        }
    }
}
//=================================================================
void updateCamera(Game* game, float deltaTime) {
    float speed = 720.0f;

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
}
