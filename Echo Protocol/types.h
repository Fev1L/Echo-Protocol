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
};

struct Noise {
    int x;
    int y;
    bool active;
    float timeLeft;
    float cooldown;
    int radius = 10; 
};

struct Game {
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    int GRID_W = 75;
    int GRID_H = 55;
    int centerX = 38;
    int centerY = 28;
    
    Monster monster;
    Noise noise;
};

struct State {
    int winW;
    int winH;

    Rectangle table;
    Rectangle monitor;
    std::vector <Rectangle> rooms;
};

struct App {
    Game* game;
    State* state;
    
    Uint64 lastCounter;
    double deltaTime;
};

struct Move {
    int x;
    int y;
};
