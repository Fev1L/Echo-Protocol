//
// Created by Fev1L on 20.03.2026.
//

#include "core.h"

void getNightConfig(App* app){
    NightConfig& cfg = app->game->cfg;
    switch(app->game->currentNight){
        case 1:
            cfg.monsterCount = 1;
            cfg.monsterMoveInterval = 1.0f;
            cfg.echoInterval = 2.5f;
            cfg.systemBreakChance = 0.0f;
            cfg.baitReload = 1.5f;
            cfg.REAL_SECONDS_PER_15_MIN = 5.0f;
            break;

        case 2:
            cfg.monsterCount = 1;
            cfg.monsterMoveInterval = 1.0f;
            cfg.echoInterval = 2.0f;
            cfg.systemBreakChance = 0.005f;
            cfg.baitReload = 1.5f;
            cfg.REAL_SECONDS_PER_15_MIN = 10.0f;
            break;

        case 3:
            cfg.monsterCount = 2;
            cfg.monsterMoveInterval = 0.9f;
            cfg.echoInterval = 1.5f;
            cfg.systemBreakChance = 0.01f;
            cfg.baitReload = 1.0f;
            cfg.REAL_SECONDS_PER_15_MIN = 20.0f;
            break;

        case 4:
            cfg.monsterCount = 3;
            cfg.monsterMoveInterval = 0.8f;
            cfg.echoInterval = 1.2f;
            cfg.systemBreakChance = 0.02f;
            cfg.baitReload = 0.8f;
            cfg.REAL_SECONDS_PER_15_MIN = 30.0f;
            break;

        case 5:
            cfg.monsterCount = 4;
            cfg.monsterMoveInterval = 0.7f;
            cfg.echoInterval = 1.0f;
            cfg.systemBreakChance = 0.03f;
            cfg.baitReload = 0.5f;
            cfg.REAL_SECONDS_PER_15_MIN = 40.0f;
            break;

        case 6:
            cfg.monsterCount = 5;
            cfg.monsterMoveInterval = 0.6f;
            cfg.echoInterval = 1.0f;
            cfg.systemBreakChance = 0.05f;
            cfg.baitReload = 0.3f;
            cfg.REAL_SECONDS_PER_15_MIN = 50.0f;
            break;

        case 7:
            cfg.monsterCount = 5;
            cfg.monsterMoveInterval = 0.5f;
            cfg.echoInterval = 0.5f;
            cfg.systemBreakChance = 0.10f;
            cfg.baitReload = 0.3f;
            cfg.REAL_SECONDS_PER_15_MIN = 60.0f;
            break;
    }
}