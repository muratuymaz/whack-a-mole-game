#include "game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

constexpr int TARGET_FPS = 60;
constexpr int FRAME_DELAY = 1000/ TARGET_FPS;
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
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        cout << "Renderer baslatilamadi!" << SDL_GetError() << endl;
        return false;
    }

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    // testTexture = LoadTexture("mole.png");
    moleTex = LoadTexture("mole.png");
    if (!moleTex) return false;
    
    SDL_QueryTexture(moleTex, nullptr, nullptr, &moleDst.w, &moleDst.h);
    
    isRunning = true;
    return true;
}
void game::run(){
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 freq = SDL_GetPerformanceFrequency();

    while (isRunning)
    {
        Uint64 nNow = SDL_GetPerformanceCounter();
        float dt = static_cast<float> (nNow- now)/ freq;
        now = nNow;

        processInput();
        update(dt);
        render();
        
    }
}
void game::processInput(){
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {

        switch (event.type)
        {
        case SDL_QUIT:
            cout << "Oyun kapatiliyor..." << endl;
            isRunning = false;
            break;
        case SDL_KEYDOWN:
            cout << "Tusa basildi: " << SDL_GetKeyName(event.key.keysym.sym) << endl;
            if (event.key.keysym.sym == SDLK_BACKSPACE)
            {
                cout << "Oyun kapatiliyor..." << endl;
                isRunning = false;
            }
            break;
        // case SDL_MOUSEMOTION:
        //     cout << "Mouse hareketi: (" << event.motion.x << ", " << event.motion.y << ")" << endl;
        //     break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            { 
                cout << "Mouse sol tusuna basildi: (" << event.button.x << ", " << event.button.y << ")" << endl;
            }else if (event.button.button == SDL_BUTTON_RIGHT)
            {
                cout << "Mouse sag tusuna basildi: (" << event.button.x << ", " << event.button.y << ")" << endl;
            }
            break;
        default:
            break;
        }
    }
}

void game::update(float dt){
    molePos.x += moleVel.x*dt;
    molePos.y += moleVel.y*dt;
    if (molePos.x < 0 || molePos.x + moleDst.w > windowWidth) moleVel.x *= -1;
    if (molePos.y < 0 || molePos.y + moleDst.h > windowHeight) moleVel.y *= -1;
    
}

void game::render(){
    
    SDL_SetRenderDrawColor(renderer, 125, 30, 200, 255);
    SDL_RenderClear(renderer);

    // moleDst.x = static_cast<int> (molePos.x);
    moleDst.y = static_cast<int> (molePos.y);

    SDL_RenderCopy(renderer, moleTex, nullptr, &moleDst);
    // SDL_Rect molePng {100, 200, 128, 128};
    // SDL_RenderCopy(renderer, testTexture, nullptr, &molePng);

    SDL_RenderPresent(renderer);
}

void game::shutdown(){
    if(renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if(window) 
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

SDL_Texture* game::LoadTexture(const string& path){
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface)
    {
        cerr << IMG_GetError() << endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    return texture;
}