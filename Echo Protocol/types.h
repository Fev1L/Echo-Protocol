//
//  types.h
//  Echo Protocol
//
//  Created by Fev1L on 10.01.2026.
//

#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstdlib>

constexpr float REAL_SECONDS_PER_15_MIN = 5.0f;

struct Rectangle {
    SDL_FRect rect;
    SDL_Color color;
    float radius;
    std::string label;
};

struct Text {
    SDL_FRect rect;
    SDL_Color color;
    std::string label;
    std::string textIn;
};

struct Image {
    SDL_FRect rect;
    const char* label;
    std::string texture;
};

enum class Anchor {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    CENTER
};

struct Monster {
    int x;
    int y;
    float moveTimer;
    float cooldown;
    bool visible = false;
    float visibleTime = 0.0f;
};

struct Noise {
    int x;
    int y;
    bool active;
    float timeLeft;
    float cooldown;
    int radius = 10; 
};

struct Echo {
    bool active = false;
    float radius = 0.0f;
    float speed = 12.0f;
    float timer = 0.0f;
    float interval = 2.5f;
};

struct Game {
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    int GRID_W = 75;
    int GRID_H = 55;
    int centerX = GRID_W/2;
    int centerY = GRID_H/2;
    float gameTime = 0.0f;
    int hours = 0;
    int minutes = 0;
    
    Monster monster;
    Noise noise;
    Echo echo;
};

struct State {
    int winW;
    int winH;

    Rectangle table;
    Rectangle monitor;
    std::vector <Rectangle> rooms;
};

struct Font {
    TTF_Font* font1;
    
    Text night;
    Text hours;
    Text bait;
};

struct App {
    Game* game;
    State* state;
    Font* font;
    
    Uint64 lastCounter;
    double deltaTime;
};

struct Move {
    int x;
    int y;
};
