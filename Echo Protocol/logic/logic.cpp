//
//  logic.cpp
//  Echo Protocol
//
//  Created by Fev1L on 17.01.2026.
//

#include "logic.h"

void saveProgress(Game* game){
    std::ofstream file("save.dat");
    if (file.is_open()){
        file << game->currentNight << std::endl;
        file.close();
    }
}
//=================================================================
void loadProgress(Game* game){
    std::ifstream file("save.dat");

    if (file.is_open()){
        file >> game->currentNight;
        file.close();
    }
    else{
        game->currentNight = 1;
    }
}
//=================================================================
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
bool inBounds(int x, int y) {
    return x >= 0 && y >= 0 && x < 75 && y < 55;
}
//=================================================================
std::vector<Move> getMoves(const Monster& m) {
    std::vector<Move> moves;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
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
    Noise n;
    
    if (game->noiseCooldown > 0.0f) return;

    n.x = gridX;
    n.y = gridY;
    n.active = true;
    n.timeLeft = 10.0f;
    
    game->noise.push_back(n);
    
    game->noiseCooldown = 10.0f * game->cfg.baitReload;
    std::cout<<game->noiseCooldown<<" ";
}
//=================================================================
void getTarget(const Game* game, int monsterX, int monsterY, int& tx, int& ty) {
   tx = game->centerX;
   ty = game->centerY;

   for (const auto& n : game->noise) {

       if (!n.active) continue;

       int dx = n.x - monsterX;
       int dy = n.y - monsterY;
       int dist = abs(dx) + abs(dy);

       if (dist <= n.radius) {
           tx = n.x;
           ty = n.y;
           return;
       }
   }
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
void checkEchoHit(Game* game, float deltaTime) {
    Echo& e = game->echo;
    for(auto& m : game->monsters){
        if (!e.active || !m.present) return;

        int dx = m.x - game->centerX;
        int dy = m.y - game->centerY;
        float dist = sqrtf(dx*dx + dy*dy);

        if (fabs(dist - e.radius) < 0.5f) {
            m.visible = true;
            m.visibleTime = 2.0f;
            
            m.echoX = m.x;
            m.echoY = m.y;
            m.echoMarked = true;
        }

        if (m.visible) {
            m.visibleTime -= deltaTime;
            if (m.visibleTime <= 0.0f) {
                m.visible = false;
                m.echoMarked = false;
            }
        }
    }
}
//=================================================================
void resetGame(App* app){
    *app->game = Game();
}
//=================================================================
void startNewGame(App* app) {
    resetGame(app);
    app->game->nightIntroTimer = 0.0f;
    app->gamestate = GameState::ENDSCREEN;
    getNightConfig(app);
}
//=================================================================
void loadGame(App* app) {
    resetGame(app);
    loadProgress(app->game);
    app->game->nightIntroTimer = 0.0f;
    app->gamestate = GameState::ENDSCREEN;
    getNightConfig(app);
}
//=================================================================
void getNightConfig(App* app){
    NightConfig& cfg = app->game->cfg;
    switch(app->game->currentNight){
        case 1:
            cfg.monsterCount = 1;
            cfg.monsterMoveInterval = 1.2f;
            cfg.echoInterval = 2.5f;
            cfg.systemBreakChance = 0.0f;
            cfg.baitReload = 1.5f;
            break;

        case 2:
            cfg.monsterCount = 1;
            cfg.monsterMoveInterval = 1.0f;
            cfg.echoInterval = 2.0f;
            cfg.systemBreakChance = 0.1f;
            cfg.baitReload = 1.5f;
            break;

        case 3:
            cfg.monsterCount = 2;
            cfg.monsterMoveInterval = 0.9f;
            cfg.echoInterval = 1.5f;
            cfg.systemBreakChance = 0.2f;
            cfg.baitReload = 1.0f;
            break;

        case 4:
            cfg.monsterCount = 3;
            cfg.monsterMoveInterval = 0.8f;
            cfg.echoInterval = 1.2f;
            cfg.systemBreakChance = 0.3f;
            cfg.baitReload = 0.8f;
            break;

        case 5:
            cfg.monsterCount = 4;
            cfg.monsterMoveInterval = 0.7f;
            cfg.echoInterval = 1.0f;
            cfg.systemBreakChance = 0.4f;
            cfg.baitReload = 0.5f;
            break;

        case 6:
            cfg.monsterCount = 5;
            cfg.monsterMoveInterval = 0.6f;
            cfg.echoInterval = 1.0f;
            cfg.systemBreakChance = 0.5f;
            cfg.baitReload = 0.3f;
            break;

        case 7:
            cfg.monsterCount = 5;
            cfg.monsterMoveInterval = 0.5f;
            cfg.echoInterval = 0.5f;
            cfg.systemBreakChance = 0.7f;
            cfg.baitReload = 0.3f;
            break;
    }
}
