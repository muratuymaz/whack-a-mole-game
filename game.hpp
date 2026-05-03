#pragma once

#include <SDL2/SDL.H>
#include <SDL2/SDL_image.h>
#include <iostream>
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
    SDL_Rect moleDst {0, 0, 0, 0};
    Vec2 molePos{100.f, 100.f};
    Vec2 moleVel{2.f, 1.f};

    bool isRunning = false;
    int windowWidth = 800;
    int windowHeight = 600;
    bool isFullscreen = false;
};