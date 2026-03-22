//
//  types.h
//  Echo Protocol
//
//  Created by Fev1L on 10.01.2026.
//

#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <fstream>

enum class GameState {
    MENU,
    TUTORIAL,
    PLAYING,
    ENDSCREEN,
    CUSTOMGAME,
    WINSCREEN,
    LOSESCREEN
};

enum class ViewSide {
    LEFT = 0,
    CENTER = 1,
    RIGHT = 2
};

enum class RepairType {
    NONE,
    BAIT,
    ECHO,
    TRACK,
    REBOOT
};

struct NightConfig {
    int monsterCount;
    float monsterMoveInterval;
    float echoInterval;
    float systemBreakChance;
    float baitReload;
    float spawnInterval;
    float REAL_SECONDS_PER_15_MIN;
};

struct Rectangle {
    SDL_FRect rect;
    SDL_Color color;
    std::string label;
    ViewSide side;
};

struct Text {
    SDL_FRect rect;
    SDL_Color color;
    std::string label;
    std::string textIn;
    ViewSide side;

    SDL_Texture* texture = nullptr;
    int w = 0;
    int h = 0;
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
    float monsterLiveTime;
    float monsterSpawnTime;
    bool visible = false;
    bool present;
    float visibleTime = 0.0f;
    int echoX = 0;
    int echoY = 0;
    bool echoMarked = false;
};

struct Noise {
    int x;
    int y;
    bool active;
    float timeLeft;
    int radius = 10; 
};

struct Echo {
    bool active = false;
    float radius = 0.0f;
    float speed = 12.0f;
    float timer = 0.0f;
};

struct Camera {
    float currentX = 0.0f;
    float targetX = 0.0f;
    float speed = 6.0f;
    bool isTurning = false;
    ViewSide side = ViewSide::CENTER;
};

struct System {
    bool echoSystem = true;
    bool baitSystem = true;
    bool trackingSystem = true;
    
    bool active = false;
    float timer = 0.0f;
    float duration = 2.0f;
    RepairType type = RepairType::NONE;
};

struct AudioSystem {
    SDL_AudioStream* stream = nullptr;
    SDL_AudioSpec spec{};
    Uint8* Data = nullptr;
    Uint32 Len = 0;
};

struct FlickerLight {
    float timer = 0.0f;
    float nextFlicker = 0.0f;
    float flickerDuration = 0.0f;
    float intensity = 1.0f;
    bool flickering = false;
};

struct Menu {
    int selectedIndex = 0;
    float menuFade = 0.0f;
    SDL_Texture* menuBackground;
    SDL_Texture* menuLogo;
    SDL_Texture* lampGlowTexture;
    
    Text newGame;
    Text continueGame;
    Text continueGameNight;
    Text customGame;
};

struct CustomGame {
    int customSelected = 0;

    Text customTitle;
    Text customMonsterCount;
    Text customMoveInterval;
    Text customEchoInterval;
    Text customBreakChance;
    Text customBaitReload;
    Text customTimeSpeed;
    Text customStart;
    Text customBack;
};

struct Game {
    int GRID_W = 121;
    int GRID_H = 71;
    int centerX = GRID_W/2;
    int centerY = GRID_H/2;
    float gameTime = 0.0f;
    int hours = 0;
    int minutes = 0;
    ViewSide currentView = ViewSide::CENTER;
    ViewSide targetView  = ViewSide::CENTER;
    float viewAngle = 0.0f; 
    float viewAngleTarget = 0.0f;
    int currentNight = 1;
    bool win = false;
    float nightIntroTimer = 0.0f;
    float nightIntroDuration = 4.0f;
    float noiseCooldown = 0.0f;
    bool alarmActive = false;
    float alarmTimer = 0.0f;
    float scareTimer = 0.0f;
    float nextScareTime = 0.0f;

    float loseTimer = 0.0f;
    bool loseSoundPlayed = false;
    bool loseCanExit = false;

    float tutorialTimer = 0.0f;
    int tutorialStep = 0;
    int lastTutorialStep = -1;

    bool isCustomGame = false;
    
    std::vector<Monster> monsters;
    std::vector<Noise> noise;
    Echo echo;
    Camera camera;
    System system;
    NightConfig cfg;
    NightConfig customCfg;
    FlickerLight topLamp;
    
    Menu menu;
    CustomGame customGame;
};

struct State {
    int winW;
    int winH;

    Rectangle table;
    Rectangle tableR;
    Rectangle monitor;
    Rectangle monitorR;
    SDL_Texture* gameBackgroundTexture;
    SDL_Texture* gameBackgroundTextureRight;
    SDL_Texture* gameBackgroundTextureLeftClose;
    SDL_Texture* gameBackgroundTextureLeftOpen;
    SDL_Texture* topLightTexture;
    SDL_Texture* topRedTexture;
    std::vector <Rectangle> rooms;
};

struct Font {
    TTF_Font* font1;
    TTF_Font* font2;
    TTF_Font* font3;
    
    Text night;
    Text hours;
    Text baitSystem;
    Text echoSystem;
    Text trackingSystem;
    Text rebootAll;
    Text endGameText;
    Text winTitle;
    Text winSubtitle;
    Text loseText;
    Text tutorialText;
};

struct Audio {
    AudioSystem menuBackgroundSong;
    AudioSystem mouseClick;
    AudioSystem fanAmbient;
    AudioSystem alarmSound;

    AudioSystem scare1;
    AudioSystem scare2;
    AudioSystem scare3;
    AudioSystem scare4;
    AudioSystem scare5;
    AudioSystem scare6;
    AudioSystem scare7;
};

struct App {
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    Game* game;
    State* state;
    Font* fonts;
    Audio* audio;
    
    GameState gamestate = GameState::MENU;
    Uint64 lastCounter;
    double deltaTime;
};

struct Move {
    int x;
    int y;
};
