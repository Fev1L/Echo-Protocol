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
        file << game->win << std::endl;
        file.close();
    }
}
//=================================================================
void loadProgress(Game* game){
    std::ifstream file("save.dat");

    if (file.is_open()){
        file >> game->currentNight;
        file >> game->win;
        file.close();
    }
    else{
        game->currentNight = 1;
        game->win = false;
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
    Game* game = app->game;

    game->gameTime = 0.0f;
    game->hours = 0;
    game->minutes = 0;
    game->currentView = ViewSide::CENTER;
    game->targetView = ViewSide::CENTER;
    game->viewAngle = 0.0f;
    game->viewAngleTarget = 0.0f;
    game->nightIntroTimer = 0.0f;
    game->noiseCooldown = 0.0f;

    game->monsters.clear();
    game->noise.clear();

    game->echo = Echo();
    game->camera = Camera();
    game->system = System();
    SDL_ClearAudioStream(app->audio->menuBackgroundSong.stream);
    SDL_ClearAudioStream(app->audio->fanAmbient.stream);
}
//=================================================================
void startNewGame(App* app) {
    resetGame(app);
    app->game->currentNight = 1;
    app->fonts->endGameText.textIn = "NIGHT " + std::to_string(app->game->currentNight);
    buildText(app->renderer, app->fonts->font1, app->fonts->endGameText);
    app->fonts->night = {{layoutText(0.006f, 0.009f, app->state->winW, app->state->winH)},
        {255,255,255,255},"Night", "NIGHT " + std::to_string(app->game->currentNight), ViewSide::CENTER};
    buildText(app->renderer, app->fonts->font1, app->fonts->night);
    app->game->tutorialTimer = 0.0f;
    app->game->tutorialStep = 0;
    app->game->nightIntroTimer = 0.0f;
    app->gamestate = GameState::TUTORIAL;
    getNightConfig(app);
    app->game->scareTimer = 0.0f;
    app->game->nextScareTime = 40.0f + static_cast<float>(rand() % 60);
}
//=================================================================
void loadGame(App* app) {
    resetGame(app);
    loadProgress(app->game);
    app->fonts->endGameText.textIn = "NIGHT " + std::to_string(app->game->currentNight);
    buildText(app->renderer, app->fonts->font1, app->fonts->endGameText);
    app->fonts->night = {{layoutText(0.006f, 0.009f, app->state->winW, app->state->winH)},
        {255,255,255,255},"Night", "NIGHT " + std::to_string(app->game->currentNight), ViewSide::CENTER};
    buildText(app->renderer, app->fonts->font1, app->fonts->night);
    app->game->nightIntroTimer = 0.0f;
    renderGame(app->game, app);
    app->gamestate = GameState::ENDSCREEN;
    SDL_ClearAudioStream(app->audio->fanAmbient.stream);
    getNightConfig(app);
    app->game->scareTimer = 0.0f;
    app->game->nextScareTime = 40.0f + static_cast<float>(rand() % 60);
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
//=================================================================
void playRandomScareSound(App* app) {
    int pick = rand() % 7;

    switch (pick) {
    case 0:
        SDL_PutAudioStreamData(app->audio->scare1.stream, app->audio->scare1.Data, app->audio->scare1.Len);
        break;
    case 1:
        SDL_PutAudioStreamData(app->audio->scare2.stream, app->audio->scare2.Data, app->audio->scare2.Len);
        break;
    case 2:
        SDL_PutAudioStreamData(app->audio->scare3.stream, app->audio->scare3.Data, app->audio->scare3.Len);
        break;
    case 3:
        SDL_PutAudioStreamData(app->audio->scare4.stream, app->audio->scare4.Data, app->audio->scare4.Len);
        break;
    case 4:
        SDL_PutAudioStreamData(app->audio->scare5.stream, app->audio->scare5.Data, app->audio->scare5.Len);
        break;
    case 5:
        SDL_PutAudioStreamData(app->audio->scare6.stream, app->audio->scare6.Data, app->audio->scare6.Len);
        break;
    case 6:
        SDL_PutAudioStreamData(app->audio->scare7.stream, app->audio->scare7.Data, app->audio->scare7.Len);
        break;
    }
}

void updateScareSounds(App* app, float deltaTime) {
    Game* game = app->game;

    game->scareTimer += deltaTime;

    if (game->scareTimer >= game->nextScareTime) {
        float baseChance = 0.35f;
        float bonusChance = 0.0f;

        for (const auto& m : game->monsters) {
            if (!m.present) continue;

            int d = abs(m.x - game->centerX) + abs(m.y - game->centerY);

            if (d < 20) {
                bonusChance += 0.10f;
            }
            if (d < 12) {
                bonusChance += 0.15f;
            }
            if (d < 6) {
                bonusChance += 0.20f;
            }
        }

        float finalChance = SDL_clamp(baseChance + bonusChance, 0.0f, 0.9f);
        float roll = static_cast<float>(rand()) / RAND_MAX;

        if (roll < finalChance) {
            playRandomScareSound(app);
        }

        game->scareTimer = 0.0f;
        game->nextScareTime = 10.0f + static_cast<float>(rand() % 16);
    }
}
//=================================================================
void resetCustomConfig(Game* game) {
    game->customCfg.monsterCount = 3;
    game->customCfg.monsterMoveInterval = 0.8f;
    game->customCfg.echoInterval = 1.2f;
    game->customCfg.systemBreakChance = 0.15f;
    game->customCfg.baitReload = 0.8f;
    game->customCfg.REAL_SECONDS_PER_15_MIN = 30.0f;
}
//=================================================================
std::string formatFloat(float value, int precision = 1) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(precision) << value;
    return ss.str();
}

void refreshCustomGameTexts(App* app) {
    Game* game = app->game;

    SDL_Color normal   = {255, 255, 255, 255};
    SDL_Color selected = {255, 255,   0, 255};

    game->customGame.customTitle.textIn = "CUSTOM NIGHT";
    game->customGame.customTitle.color = normal;
    buildText(app->renderer, app->fonts->font2, game->customGame.customTitle);

    game->customGame.customMonsterCount.textIn =
        "MONSTER COUNT: " + std::to_string(game->customCfg.monsterCount);
    game->customGame.customMonsterCount.color =
        (game->customGame.customSelected == 0) ? selected : normal;
    buildText(app->renderer, app->fonts->font1, game->customGame.customMonsterCount);

    game->customGame.customMoveInterval.textIn =
        "MOVE INTERVAL: " + formatFloat(game->customCfg.monsterMoveInterval, 1);
    game->customGame.customMoveInterval.color =
        (game->customGame.customSelected == 1) ? selected : normal;
    buildText(app->renderer, app->fonts->font1, game->customGame.customMoveInterval);

    game->customGame.customEchoInterval.textIn =
        "ECHO INTERVAL: " + formatFloat(game->customCfg.echoInterval, 1);
    game->customGame.customEchoInterval.color =
        (game->customGame.customSelected == 2) ? selected : normal;
    buildText(app->renderer, app->fonts->font1, game->customGame.customEchoInterval);

    game->customGame.customBreakChance.textIn =
        "BREAK CHANCE: " + formatFloat(game->customCfg.systemBreakChance, 2);
    game->customGame.customBreakChance.color =
        (game->customGame.customSelected == 3) ? selected : normal;
    buildText(app->renderer, app->fonts->font1, game->customGame.customBreakChance);

    game->customGame.customBaitReload.textIn =
        "BAIT RELOAD: " + formatFloat(game->customCfg.baitReload, 1);
    game->customGame.customBaitReload.color =
        (game->customGame.customSelected == 4) ? selected : normal;
    buildText(app->renderer, app->fonts->font1, game->customGame.customBaitReload);

    game->customGame.customTimeSpeed.textIn =
        "TIME SPEED: " + formatFloat(game->customCfg.REAL_SECONDS_PER_15_MIN, 1);
    game->customGame.customTimeSpeed.color =
        (game->customGame.customSelected == 5) ? selected : normal;
    buildText(app->renderer, app->fonts->font1, game->customGame.customTimeSpeed);

    game->customGame.customStart.textIn = "START";
    game->customGame.customStart.color =
        (game->customGame.customSelected == 6) ? selected : normal;
    buildText(app->renderer, app->fonts->font2, game->customGame.customStart);

    game->customGame.customBack.textIn = "BACK";
    game->customGame.customBack.color =
        (game->customGame.customSelected == 7) ? selected : normal;
    buildText(app->renderer, app->fonts->font1, game->customGame.customBack);
}