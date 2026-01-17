//
//  logic.cpp
//  Echo Protocol
//
//  Created by Fev1L on 17.01.2026.
//

#include "logic.h"

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
    n.cooldown = 60.0f;
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
void updateMonster(Monster& m, const Game* game,float deltaTime) {
    m.moveTimer += deltaTime;
    if (m.moveTimer >= 0.5f) {
        Move next = chooseMoveProb(m, game);
        m.x = next.x;
        m.y = next.y;
        m.moveTimer = 0.0f;
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
