#include "game.hpp"
#include <iostream>
using namespace std;

game::game(){};
game::~game(){
    shutdown();
};

bool game::init(string name, int windowWidth, int windowHeight){
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        cout << "SDL baslatilamadi!" << SDL_GetError() << endl;
        return false;
    }
    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window)
    {
        cout<< "Pencere baslatilamadi!" << SDL_GetError() << endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        cout << "Renderer baslatilamadi!" << SDL_GetError() << endl;
        return false;
    }
    isRunning = true;
    return true;
}
void game::run(){
    while (isRunning)
    {
        processInput();
        update(0.0f);
        render();
    }
}
void game::processInput(){
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
    }
}

void game::update(float deltaTime){
    
}

void game::render(){
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void game::shutdown(){
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
    SDL_Quit();
}