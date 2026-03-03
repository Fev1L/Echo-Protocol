//
//  init.cpp
//  Echo Protocol
//
//  Created by Fev1L on 25.02.2026.
//

#include "init.h"

bool initApp(App* app){
    Game* game = app->game;
    State* state = app->state;
    Font* fonts = app->fonts;
    Audio* audio = app->audio;
    
    //=================INIT=================
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Failure!");
        return false;
    }
    
    if (!TTF_Init()) {
        SDL_Log("Failure!");
        return false;
    }
    
    app->window = SDL_CreateWindow("Protocol 'ECHO'", 1920, 1080, SDL_WINDOW_FULLSCREEN);
    app->renderer = SDL_CreateRenderer(app->window, nullptr);
    
    SDL_GetWindowSize(app->window, &state->winW, &state->winH);
    //=================INIT=================
    
    //=================FONTS=================
    std::string basePath = SDL_GetBasePath();
    std::string fontPath = basePath + "Assets/Inter_28pt-Regular.ttf";
    std::string fontPath1 = basePath + "Assets/Inter_28pt-ExtraBold.ttf";
    fonts->font1 = TTF_OpenFont(fontPath.c_str(), 24);
    fonts->font2 = TTF_OpenFont(fontPath.c_str(), 64);
    fonts->font3 = TTF_OpenFont(fontPath1.c_str(), 96);
    //=================FONTS=================
    
    //=================AUDIO=================
    SDL_LoadWAV("assets/sounds/menuBackgroundSong.wav", &audio->menuBackgroundSong.spec, &audio->menuBackgroundSong.Data, &audio->menuBackgroundSong.Len);
    audio->menuBackgroundSong.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio->menuBackgroundSong.spec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(audio->menuBackgroundSong.stream);
    
    SDL_LoadWAV("assets/sounds/mouseClick.wav", &audio->mouseClick.spec, &audio->mouseClick.Data, &audio->mouseClick.Len);
    audio->mouseClick.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio->mouseClick.spec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(audio->mouseClick.stream);
    SDL_SetAudioStreamGain(app->audio->mouseClick.stream, 0.5f);
    //=================AUDIO=================
    
    //=================IMAGE=================
    game->menu.menuBackground = IMG_LoadTexture(app->renderer, "Assets/menu.png");
    app->state->gameBackgroundTexture = IMG_LoadTexture(app->renderer, "Assets/gameBackground.png");
    app->state->gameBackgroundTextureRight = IMG_LoadTexture(app->renderer, "Assets/gameBackgroundRight.png");
    app->state->gameBackgroundTextureLeftClose = IMG_LoadTexture(app->renderer, "Assets/gameBackgroundLeftClose.png");
    app->state->gameBackgroundTextureLeftOpen = IMG_LoadTexture(app->renderer, "Assets/gameBackgroundLeftOpen.png");
    game->menu.menuLogo = IMG_LoadTexture(app->renderer, "Assets/LOGO.png");
    game->menu.lampGlowTexture = IMG_LoadTexture(app->renderer, "Assets/lampGlowTexture.png");
    SDL_SetTextureBlendMode(game->menu.lampGlowTexture, SDL_BLENDMODE_ADD);
    //=================IMAGE=================
    
    //=================RECT=================
    game->menu.newGame = {{layoutText(0.034f, 0.5f, app->state->winW, app->state->winH)}, {255,255,255,255},"newGame", "NEW GAME", ViewSide::CENTER};
    game->menu.continueGame = {{layoutText(0.034f, 0.589f, app->state->winW, app->state->winH)}, {255,255,255,255},"continueGame", "CONTINUE GAME", ViewSide::CENTER};
    game->menu.continueGameNight = {{layoutText(0.046f, 0.661f, app->state->winW, app->state->winH)}, {255,255,255,255},"continueGameNight", "NIGHT " + std::to_string(game->currentNight), ViewSide::CENTER};
    game->menu.customGame = {{layoutText(0.034f, 0.706f, app->state->winW, app->state->winH)}, {255,255,255,255},"customNight", "CUSTOM NIGHT", ViewSide::CENTER};
    
    fonts->baitSystem = {{layoutText(0.281f, 0.296f, state->winW, state->winH)}, {0,255,0,255},"baitSystem", "BAIT SYSTEM", ViewSide::RIGHT};
    fonts->echoSystem = {{layoutText(0.281f, 0.346f, state->winW, state->winH)}, {0,255,0,255},"echoSystem", "ECHO SYSTEM", ViewSide::RIGHT};
    fonts->trackingSystem = {{layoutText(0.281f, 0.395f, state->winW, state->winH)}, {0,255,0,255},"trackingSystem", "TRACKING SYSTEM", ViewSide::RIGHT};
    fonts->rebootAll = {{layoutText(0.281f, 0.5f, state->winW, state->winH)}, {0,255,0,255},"rebootAll", "REBOOT ALL", ViewSide::RIGHT};
    
    app->fonts->endGameText = {{layoutText(0.45f, 0.5f, app->state->winW, app->state->winH)}, {255,255,255,255},"endGame", "NIGHT " + std::to_string(game->currentNight), ViewSide::CENTER};
    //=================RECT=================
    
    return true;
}
