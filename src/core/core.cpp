//
//  init.cpp
//  Echo Protocol
//
//  Created by Fev1L on 25.02.2026.
//

#include "core.h"

bool initApp(App* app){
    Game* game = app->game;
    State* state = app->state;
    Font* fonts = app->fonts;
    Audio* audio = app->audio;
    
    //=================INIT=================
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL Init error: %s", SDL_GetError());
        return false;
    }
    
    if (!TTF_Init()) {
        SDL_Log("TTF Init error: %s", SDL_GetError());
        return false;
    }
    
    app->window = SDL_CreateWindow("Protocol 'ECHO'", 700, 700, SDL_WINDOW_FULLSCREEN);
    app->renderer = SDL_CreateRenderer(app->window, nullptr);
    
    SDL_GetWindowSize(app->window, &state->winW, &state->winH);
    buildGrid(app);
    state->rooms.shrink_to_fit();
    //=================INIT=================
    
    //=================FONTS=================
    const std::string basePath = SDL_GetBasePath();
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

    SDL_LoadWAV("assets/sounds/fanAmbient.wav", &audio->fanAmbient.spec, &audio->fanAmbient.Data, &audio->fanAmbient.Len);
    audio->fanAmbient.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio->fanAmbient.spec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(audio->fanAmbient.stream);
    SDL_SetAudioStreamGain(app->audio->fanAmbient.stream, 2.0f);

    SDL_LoadWAV("Assets/sounds/alarm.wav", &audio->alarmSound.spec, &audio->alarmSound.Data, &audio->alarmSound.Len);
    audio->alarmSound.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio->alarmSound.spec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(audio->alarmSound.stream);
    SDL_SetAudioStreamGain(audio->alarmSound.stream, 2.0f);

    SDL_LoadWAV("Assets/sounds/scare1.wav", &audio->scare1.spec, &audio->scare1.Data, &audio->scare1.Len);
    audio->scare1.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio->scare1.spec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(audio->scare1.stream);

    SDL_LoadWAV("Assets/sounds/scare2.wav", &audio->scare2.spec, &audio->scare2.Data, &audio->scare2.Len);
    audio->scare2.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio->scare2.spec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(audio->scare2.stream);

    SDL_LoadWAV("Assets/sounds/scare3.wav", &audio->scare3.spec, &audio->scare3.Data, &audio->scare3.Len);
    audio->scare3.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio->scare3.spec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(audio->scare3.stream);

    SDL_LoadWAV("Assets/sounds/scare4.wav", &audio->scare4.spec, &audio->scare4.Data, &audio->scare4.Len);
    audio->scare4.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio->scare4.spec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(audio->scare4.stream);

    SDL_LoadWAV("Assets/sounds/scare5.wav", &audio->scare5.spec, &audio->scare5.Data, &audio->scare5.Len);
    audio->scare5.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio->scare5.spec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(audio->scare5.stream);

    SDL_LoadWAV("Assets/sounds/scare6.wav", &audio->scare6.spec, &audio->scare6.Data, &audio->scare6.Len);
    audio->scare6.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio->scare6.spec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(audio->scare6.stream);

    SDL_LoadWAV("Assets/sounds/scare7.wav", &audio->scare7.spec, &audio->scare7.Data, &audio->scare7.Len);
    audio->scare7.stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio->scare7.spec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(audio->scare7.stream);
    //=================AUDIO=================
    
    //=================IMAGE=================
    game->menu.menuBackground = IMG_LoadTexture(app->renderer, "Assets/menu.png");
    app->state->gameBackgroundTexture = IMG_LoadTexture(app->renderer, "Assets/gameBackground.png");
    app->state->gameBackgroundTextureRight = IMG_LoadTexture(app->renderer, "Assets/gameBackgroundRight.png");
    app->state->gameBackgroundTextureLeftClose = IMG_LoadTexture(app->renderer, "Assets/gameBackgroundLeftClose.png");
    app->state->gameBackgroundTextureLeftOpen = IMG_LoadTexture(app->renderer, "Assets/gameBackgroundLeftOpen.png");
    app->state->topLightTexture = IMG_LoadTexture(app->renderer, "Assets/lampTopYellow.png");
    app->state->topRedTexture = IMG_LoadTexture(app->renderer, "Assets/lampTopRed.png");
    game->menu.menuLogo = IMG_LoadTexture(app->renderer, "Assets/LOGO.png");
    game->menu.lampGlowTexture = IMG_LoadTexture(app->renderer, "Assets/lampGlowTexture.png");
    SDL_SetTextureBlendMode(game->menu.lampGlowTexture, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(app->state->topLightTexture, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(app->state->topRedTexture, SDL_BLENDMODE_ADD);
    //=================IMAGE=================
    
    //=================RECT=================
    game->menu.newGame = {{layoutText(0.034f, 0.5f, app->state->winW, app->state->winH)}, {255,255,255,255},"newGame", "NEW GAME", ViewSide::CENTER};
    buildText(app->renderer, fonts->font2, game->menu.newGame);
    game->menu.continueGame = {{layoutText(0.034f, 0.589f, app->state->winW, app->state->winH)}, {255,255,255,255},"continueGame", "CONTINUE GAME", ViewSide::CENTER};
    buildText(app->renderer, fonts->font2, game->menu.continueGame);
    game->menu.continueGameNight = {{layoutText(0.046f, 0.661f, app->state->winW, app->state->winH)}, {255,255,255,255},"continueGameNight", "NIGHT " + std::to_string(game->currentNight), ViewSide::CENTER};
    buildText(app->renderer, fonts->font1, game->menu.continueGameNight);
    game->menu.customGame = {{layoutText(0.034f, 0.706f, app->state->winW, app->state->winH)}, {255,255,255,255},"customNight", "CUSTOM NIGHT", ViewSide::CENTER};
    buildText(app->renderer, fonts->font2, game->menu.customGame);

    fonts->baitSystem = {{layoutText(0.281f, 0.218f, state->winW, state->winH)}, {0,255,0,255},"baitSystem", "BAIT SYSTEM", ViewSide::RIGHT};
    buildText(app->renderer, fonts->font1, fonts->baitSystem);
    fonts->echoSystem = {{layoutText(0.281f, 0.268f, state->winW, state->winH)}, {0,255,0,255},"echoSystem", "ECHO SYSTEM", ViewSide::RIGHT};
    buildText(app->renderer, fonts->font1, fonts->echoSystem);
    fonts->trackingSystem = {{layoutText(0.281f, 0.317f, state->winW, state->winH)}, {0,255,0,255},"trackingSystem", "TRACKING SYSTEM", ViewSide::RIGHT};
    buildText(app->renderer, fonts->font1, fonts->trackingSystem);
    fonts->rebootAll = {{layoutText(0.281f, 0.422f, state->winW, state->winH)}, {0,255,0,255},"rebootAll", "REBOOT ALL", ViewSide::RIGHT};
    buildText(app->renderer, fonts->font1, fonts->rebootAll);
    fonts->hours = {{layoutText(0.006f, 0.029f, state->winW, state->winH)}, {255,255,255,255},"Hours", "HOURS", ViewSide::CENTER};
    buildText(app->renderer, fonts->font1, fonts->hours);

    app->fonts->endGameText = {{layoutText(0.45f, 0.5f, app->state->winW, app->state->winH)}, {255,255,255,255},"endGame", "NIGHT " + std::to_string(game->currentNight), ViewSide::CENTER};

    fonts->winTitle = {{layoutText(0.266f, 0.435f, state->winW, state->winH)}, {255,255,255,255}, "winTitle", "THANK YOU FOR PLAYING", ViewSide::CENTER};
    buildText(app->renderer, fonts->font2, fonts->winTitle);
    fonts->winSubtitle = {{layoutText(0.284f, 0.492f, state->winW, state->winH)}, {180,180,180,255}, "winSubtitle", "You survived all 7 nights", ViewSide::CENTER};
    buildText(app->renderer, fonts->font2, fonts->winSubtitle);
    //=================RECT=================
    
    return true;
}
