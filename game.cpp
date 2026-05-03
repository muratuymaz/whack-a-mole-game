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
    
    int rows = 3;
    int cols = 3;
    int holeSize = 100; 
    
    int spacingX = (windowWidth - (cols * holeSize)) / (cols + 1);
    int spacingY = (windowHeight - (rows * holeSize)) / (rows + 1);

    int index = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            holes[index].w = holeSize;
            holes[index].h = holeSize;

            holes[index].x = spacingX + j * (holeSize + spacingX);
            holes[index].y = spacingY + i * (holeSize + spacingY);
            index++;
        }
    }

    isRunning = true;
    return true;
}
void game::run(){
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 freq = SDL_GetPerformanceFrequency();

    while (isRunning)
    {
        Uint64 nNow = SDL_GetPerformanceCounter();
        float dt = static_cast<float> (nNow - now) / freq;
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
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                SDL_Point mousePoint = { mouseX, mouseY };
                if (SDL_PointInRect(&mousePoint, &moleDst) && isVisible) 
                {
                    cout<< "vurdun" << endl;
                    isVisible = false;
                }
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

    SDL_SetRenderDrawColor(renderer, 50, 25, 10, 255); 
    for (int i = 0; i < 9; ++i) {
        SDL_RenderFillRect(renderer, &holes[i]); 
    }

    if (isVisible)
    {
        for (int i = 0; i < 9; ++i) {
            SDL_Rect currentMoleDst;
            
            currentMoleDst.w = static_cast<int>(holes[i].w * 0.8); 
            currentMoleDst.h = static_cast<int>(holes[i].h * 0.8); 
                        
            currentMoleDst.x = holes[i].x + (holes[i].w - currentMoleDst.w) / 2;
            currentMoleDst.y = holes[i].y + (holes[i].h - currentMoleDst.h) / 2;

            SDL_RenderCopy(renderer, moleTex, nullptr, &currentMoleDst);
        }
    }
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