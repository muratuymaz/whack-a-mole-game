#pragma once

#include <SDL2/SDL.H>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL_ttf.h>
#include <string>
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
    // SDL_Texture* testTexture = nullptr;
    
    struct Vec2 {float x{0.f}, y{0.f};};
    SDL_Texture* moleTex = nullptr;
    SDL_Texture* backgroundTex = nullptr;
    SDL_Rect moleDst {0, 0, 0 ,0}; // x, y, w, h
    Vec2 molePos{0.f, 0.f};
    Vec2 moleVel{180.f, 90.f};

    SDL_Rect holes[9];
    SDL_Rect moles[9];
    bool isVisible[9];

    float spawnTimer = 0.0f;
    float currentSpawnDelay = 1.0f;
    float moleTimers[9] = {0.0f};

    int score = 0;
    TTF_Font* font;
    string scoreText;
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* textSurface = nullptr;
    SDL_Texture* textTexture = nullptr;
    SDL_Rect textRect;

    bool isRunning = false;
    int windowWidth = 800;
    int windowHeight = 600;
};