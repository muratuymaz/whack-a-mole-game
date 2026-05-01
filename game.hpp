#pragma once

#include <SDL2/SDL.H>
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
    bool isRunning = false;
    int windowWidth = 800;
    int windowHeight = 600;
};