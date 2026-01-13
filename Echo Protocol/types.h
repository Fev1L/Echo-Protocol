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

struct Bavel {
    SDL_FRect rect;
    int index[2];
    std::string label;
    bool revealed = false;
    bool flag = false;
};

struct Text {
    SDL_FRect rect;
    SDL_Color color;
    std::string label;
    std::string textIn;
};

struct Triangle {
    SDL_FPoint p1;
    SDL_FPoint p2;
    SDL_FPoint p3;
    SDL_Color color;
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

struct Game {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

struct State {
    int winW;
    int winH;
    
    Rectangle quit;
};

struct App {
    Game* game;
    State* state;
};
