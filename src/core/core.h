//
//  init.h
//  Echo Protocol
//
//  Created by Fev1L on 26.02.2026.
//

#pragma once
#include "../types.h"

bool initApp(App* app);

// LOGIC

void saveProgress(Game* game);

void loadProgress(Game* game);

bool inBounds(int x, int y, int GRID_W, int GRID_H);

std::vector<Move> getMoves(const Monster& m, Game* game);

inline int distance(int x1, int y1, int x2, int y2);

void getTarget(const Game* game, int monsterX, int monsterY, int& tx, int& ty);

Move chooseMoveProb(const Monster& m, Game* game);

void checkEchoHit(Game* game, float deltaTime);

void resetGame(App* app);

void startNewGame(App* app);

void loadGame(App* app);

void updateSystemColor(SDL_Renderer* renderer, TTF_Font* font, Text& text, SDL_Color targetColor);

void getNightConfig(App* app);

void updateGameClock(Game* game, float deltaTime);

void updateRepair(Game* game, float dt);

bool hasBrokenSystem(const Game* game);

void updateAlarm(Game* game, float deltaTime);

void updateScareSounds(App* app, float deltaTime);

void resetCustomConfig(Game* game);

void refreshCustomGameTexts(App* app);

void updateTutorialText(App* app);

// RENDER

void drawRectangle(SDL_Renderer* renderer,const Rectangle &rct, App* app);

bool isButtonClicked(SDL_FRect rect, float x, float y);

bool isTextClicked(SDL_Renderer* renderer , TTF_Font* font, const Text& text, const App* app, float x, float y);

void buildText(SDL_Renderer* renderer, TTF_Font* font, Text& text);

void drawText(SDL_Renderer* renderer, const Text& text, const App* app);

void drawImage(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_FRect& targetRect, App* app , ViewSide side);

SDL_FRect layout(Anchor anchor, float wPct, float hPct, float marginXPct, float marginYPct, int winW, int winH);

SDL_FRect layoutText(float marginXPct, float marginYPct, int winW, int winH);

float sideOffsetX(ViewSide side, float screenW);

void buildGrid(App* app);