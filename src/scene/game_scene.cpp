//
// Created by Fev1L on 18.03.2026.
//

#include "scene.h"

void renderGame(Game* game, App* app) {
    State* state = app->state;
    Font* fonts = app->fonts;

    if (SDL_GetAudioStreamQueued(app->audio->fanAmbient.stream) < (int)app->audio->fanAmbient.Len)
        SDL_PutAudioStreamData(app->audio->fanAmbient.stream, app->audio->fanAmbient.Data, app->audio->fanAmbient.Len);

    SDL_FRect gameBackgroundRect = {0, 0, static_cast<float>(app->state->winW), static_cast<float>(app->state->winH)};
    drawImage(app->renderer, app->state->gameBackgroundTexture, gameBackgroundRect,app, ViewSide::CENTER);
    drawImage(app->renderer, app->state->gameBackgroundTextureRight, gameBackgroundRect,app, ViewSide::RIGHT);
    drawImage(app->renderer, app->state->gameBackgroundTextureLeftClose, gameBackgroundRect,app, ViewSide::LEFT);

    constexpr SDL_Color red = {255, 0, 0, 255};
    constexpr SDL_Color green = {0, 255, 0, 255};
    constexpr SDL_Color yellow = {255, 255, 0, 255};

    SDL_Color baitColor;
    SDL_Color echoColor;
    SDL_Color trackColor;

    /* BAIT */
    if (game->system.type == RepairType::BAIT)
        baitColor = yellow;
    else if (game->system.baitSystem)
        baitColor = green;
    else
        baitColor = red;
    /* ECHO */
    if (game->system.type == RepairType::ECHO)
        echoColor = yellow;
    else if (game->system.echoSystem)
        echoColor = green;
    else
        echoColor = red;
    /* TRACK */
    if (game->system.type == RepairType::TRACK)
        trackColor = yellow;
    else if (game->system.trackingSystem)
        trackColor = green;
    else
        trackColor = red;
    /* REBOOT */
    if (game->system.type == RepairType::REBOOT)
    {
        baitColor = yellow;
        echoColor = yellow;
        trackColor = yellow;
    }

    updateSystemColor(app->renderer, fonts->font1, fonts->baitSystem, baitColor);
    updateSystemColor(app->renderer, fonts->font1, fonts->echoSystem, echoColor);
    updateSystemColor(app->renderer, fonts->font1, fonts->trackingSystem, trackColor);

    bool echoDetectedMonster = false;

    if (game->echo.active) {
        for (const auto& monster : game->monsters) {
            if (!monster.present) continue;

            int mdx = monster.x - game->centerX;
            int mdy = monster.y - game->centerY;
            float monsterDist = sqrtf(mdx * mdx + mdy * mdy);

            if (fabs(monsterDist - game->echo.radius) < 0.5f) {
                echoDetectedMonster = true;
                break;
            }
        }
    }

    for(int i = 0; i < game->GRID_H; i++) {
        for(int j = 0; j < game->GRID_W; j++) {
            int idx = i * game->GRID_W + j;
            Rectangle& cell = state->rooms[idx];

            float baseR = 11.0f;
            float baseG = 47.0f;
            float baseB = 0.0f;

            float lightR = 0.0f;
            float lightG = 0.0f;
            float lightB = 0.0f;

            for (const auto& noise : game->noise) {
                if (!noise.active) continue;

                int dx = j - noise.x;
                int dy = i - noise.y;

                float dist2 = dx * dx + dy * dy;
                float r2 = noise.radius * noise.radius;

                if (dist2 <= r2) {
                    float dist = sqrtf(dist2);
                    float t = 1.0f - (dist / noise.radius);
                    t = t * t;

                    lightR += 160.0f * t;
                    lightG += 170.0f * t;
                    lightB += 120.0f * t;
                }
            }

            bool isEchoRingCell = false;

            if (game->echo.active) {
                int dx = j - game->centerX;
                int dy = i - game->centerY;

                float dist = sqrtf(dx * dx + dy * dy);
                float diff = fabs(dist - game->echo.radius);

                if (diff < 1.0f) {
                    isEchoRingCell = true;
                    float t = 1.0f - diff;

                    lightR += 255.0f * t;
                    lightG += 255.0f * t;
                    lightB += 255.0f * t;
                }
            }

            float finalR = SDL_clamp(baseR + lightR, 0.0f, 255.0f);
            float finalG = SDL_clamp(baseG + lightG, 0.0f, 255.0f);
            float finalB = SDL_clamp(baseB + lightB, 0.0f, 255.0f);

            SDL_Color color = {
                (Uint8)finalR,
                (Uint8)finalG,
                (Uint8)finalB,
                255
            };

            if (game->system.trackingSystem) {
                for (const auto& monster : game->monsters) {
                    if (monster.present && monster.visible &&
                        i == monster.echoY && j == monster.echoX) {
                        color = {255, 60, 60, 255};
                        break;
                    }
                }
            } else {
                if (echoDetectedMonster && isEchoRingCell) {
                    color = {255, 60, 60, 255};
                }
            }

            if (i == game->centerY && j == game->centerX) {
                color = {0, 255, 0, 255};
            }

            cell.color = color;
        }
    }

    for(const Rectangle rec : state->rooms){
        drawRectangle(app->renderer, rec, app);
    }
    drawText(app->renderer, fonts->night, app);

    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << game->hours
       << ":"
       << std::setw(2) << std::setfill('0') << game->minutes;


    if (fonts->hours.textIn != ss.str()){
        fonts->hours.textIn = ss.str();
        buildText(app->renderer, fonts->font1, fonts->hours);
    }

    drawText(app->renderer, fonts->hours, app);

    drawText(app->renderer, fonts->baitSystem, app);
    drawText(app->renderer, fonts->echoSystem, app);
    drawText(app->renderer, fonts->trackingSystem, app);
    drawText(app->renderer, fonts->rebootAll, app);
}
