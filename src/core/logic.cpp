//
//  logic.cpp
//  Echo Protocol
//
//  Created by Fev1L on 17.01.2026.
//

#include "core.h"

#include "scene/scene.h"

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
bool inBounds(int x, int y, int GRID_W, int GRID_H) {
    return x >= 0 && y >= 0 && x < GRID_W && y < GRID_H;
}
//=================================================================
std::vector<Move> getMoves(const Monster& m, Game* game) {
    std::vector<Move> moves;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = m.x + dx;
            int ny = m.y + dy;
            if (inBounds(nx, ny, game->GRID_W, game->GRID_H)) {
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
Move chooseMoveProb(const Monster& m, Game* game) {
    int tx, ty;
    getTarget(game, m.x, m.y, tx, ty);

    auto moves = getMoves(m, game);

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

    if (!e.active) {
        for (auto& m : game->monsters) {
            m.echoMarked = false;
            if (m.visible) {
                m.visibleTime -= deltaTime;
                if (m.visibleTime <= 0.0f) {
                    m.visible = false;
                }
            }
        }
        return;
    }

    for (auto& m : game->monsters) {
        if (!m.present) continue;

        int dx = m.x - game->centerX;
        int dy = m.y - game->centerY;
        float dist = sqrtf(dx * dx + dy * dy);

        if (!m.echoMarked && fabs(dist - e.radius) < 0.5f) {
            m.echoMarked = true;

            float r = static_cast<float>(rand()) / RAND_MAX;
            if (r < game->cfg.systemBreakChance) {
                game->system.trackingSystem = false;
            }

            if (game->system.trackingSystem) {
                m.visible = true;
                m.visibleTime = 2.0f;
                m.echoX = m.x;
                m.echoY = m.y;
            }
        }

        if (m.visible) {
            m.visibleTime -= deltaTime;
            if (m.visibleTime <= 0.0f) {
                m.visible = false;
            }
        }
    }
}
//=================================================================
void resetGame(App* app){
    *app->game = Game();
    SDL_ClearAudioStream(app->audio->menuBackgroundSong.stream);
}
//=================================================================
void startNewGame(App* app) {
    resetGame(app);
    app->fonts->night = {{layoutText(0.006f, 0.009f, app->state->winW, app->state->winH)},
        {255,255,255,255},"Night", "NIGHT " + std::to_string(app->game->currentNight), ViewSide::CENTER};
    buildText(app->renderer, app->fonts->font1, app->fonts->night);
    app->game->nightIntroTimer = 0.0f;
    renderGame(app->game, app);
    app->gamestate = GameState::ENDSCREEN;
    getNightConfig(app);
}
//=================================================================
void loadGame(App* app) {
    resetGame(app);
    loadProgress(app->game);
    app->fonts->night = {{layoutText(0.006f, 0.009f, app->state->winW, app->state->winH)},
        {255,255,255,255},"Night", "NIGHT " + std::to_string(app->game->currentNight), ViewSide::CENTER};
    buildText(app->renderer, app->fonts->font1, app->fonts->night);
    app->game->nightIntroTimer = 0.0f;
    renderGame(app->game, app);
    app->gamestate = GameState::ENDSCREEN;
    SDL_ClearAudioStream(app->audio->fanAmbient.stream);
    getNightConfig(app);
}
//=================================================================
void updateSystemColor(SDL_Renderer* renderer, TTF_Font* font, Text& text, SDL_Color targetColor){
    if (text.color.r != targetColor.r ||
        text.color.g != targetColor.g ||
        text.color.b != targetColor.b)
    {
        text.color = targetColor;
        buildText(renderer, font, text);
    }
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
//=================================================================
bool hasBrokenSystem(const Game* game) {
    return !game->system.baitSystem ||
           !game->system.echoSystem ||
           !game->system.trackingSystem;
}
//=================================================================
void updateAlarm(Game* game, float deltaTime) {
    game->alarmActive = !game->system.baitSystem ||
                        !game->system.echoSystem ||
                        !game->system.trackingSystem;

    if (game->alarmActive) {
        game->alarmTimer += deltaTime;
    } else {
        game->alarmTimer = 0.0f;
    }
}