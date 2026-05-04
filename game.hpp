#pragma once

#include <SDL2/SDL.H>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "renderQueue.h"

using namespace std;

class game{
public:
    game();
    ~game();
    bool init(string name, int windowWidth, int windowHeight);
    void shutdown();
    void run();
    
    private:
    void processInput();
    void update(float deltaTime);
    void render();
    
    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* LoadTexture(const string& path);
    
    SDL_Texture* moleTex = nullptr;
    SDL_Texture* backgroundTex = nullptr;

    renderQueue RQ;

    SDL_Rect holes[9];
    SDL_Rect moles[9];
    SDL_Texture* holeTex = nullptr;
    bool isVisible[9];

    float spawnTimer = 0.0f;
    float currentSpawnDelay = 1.0f;
    float moleTimers[9] = {0.0f};

    int score = 0;
    TTF_Font* font;

    bool isRunning = false;
    int windowWidth = 800;
    int windowHeight = 600;
};