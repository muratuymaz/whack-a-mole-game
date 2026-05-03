#include "game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <random>

constexpr int TARGET_FPS = 60;
constexpr int FRAME_DELAY = 1000/ TARGET_FPS;
using namespace std;

game::game(){};
game::~game(){
    shutdown();
};

bool game::init(string name, int windowWidth, int windowHeight){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return false;
    
    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window) return false;
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) return false;
    if (TTF_Init() == -1) return false;

    font = TTF_OpenFont("font.ttf", 24);
    if (!font) return false;

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    
    moleTex = LoadTexture("assets/mole.png");
    if (!moleTex) return false;

    backgroundTex = LoadTexture("assets/bg.png");
    if (!backgroundTex) return false;
    
    SDL_QueryTexture(moleTex, nullptr, nullptr, &moleDst.w, &moleDst.h);
    
    int rows = 3;
    int cols = 3;
    int holeSize = 100;

    int spacingX = (windowWidth - (cols * holeSize)) / (cols + 1);
    int spacingY = (windowHeight - (rows * holeSize)) / (rows + 1);

    int index = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            holes[index].w = holeSize;
            holes[index].h = holeSize;
            holes[index].x = spacingX + j * (holeSize + spacingX);
            holes[index].y = spacingY + i * (holeSize + spacingY);

            moles[index].w = static_cast<int>(holeSize);
            moles[index].h = static_cast<int>(holeSize);
            moles[index].x = holes[index].x + (holes[index].w - moles[index].w) / 2;
            moles[index].y = holes[index].y + (holes[index].h - moles[index].h) / 2;

            isVisible[index] = false; 
            moleTimers[index] = 0.0f;

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
                for (int i = 0; i < 9; ++i) {
                    if (isVisible[i] && SDL_PointInRect(&mousePoint, &moles[i])) 
                    {
                        for (int i = 0; i < 9; ++i) {
                            if (isVisible[i] && SDL_PointInRect(&mousePoint, &moles[i])) 
                            {
                                cout << i << ". kostebege vurdun!" << endl;
                                isVisible[i] = false;

                                score += 10; 
                                
                                break;
                            }
                        }
                    }
                }
            }
            break;
        default:
            break;
        }
    }
}

void game::update(float dt){
    // molePos.x += moleVel.x*dt;
    // molePos.y += moleVel.y*dt;
    // if (molePos.x < 0 || molePos.x + moleDst.w > windowWidth) moleVel.x *= -1;
    // if (molePos.y < 0 || molePos.y + moleDst.h > windowHeight) moleVel.y *= -1;
    
    spawnTimer += dt;

    if (spawnTimer >= currentSpawnDelay) {
        spawnTimer = 0.0f;
        
        currentSpawnDelay = 0.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 1.5f));

        int randomHole = rand() % 9;
        
        if (!isVisible[randomHole]) {
            isVisible[randomHole] = true;
            moleTimers[randomHole] = 0.0f;
        }
    }

    for (int i = 0; i < 9; ++i) {
        if (isVisible[i]) {
            moleTimers[i] += dt;

            if (moleTimers[i] >= 1.5f) {
                isVisible[i] = false;
            }
        }
    }
}

void game::render(){
    
    // SDL_SetRenderDrawColor(renderer, 125, 30, 200, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTex, NULL, NULL);
    
    SDL_SetRenderDrawColor(renderer, 50, 25, 10, 255); 
    for (int i = 0; i < 9; ++i) {
        SDL_RenderFillRect(renderer, &holes[i]); 
    }

    for (int i = 0; i < 9; ++i) {
        if (isVisible[i]) {
            SDL_RenderCopy(renderer, moleTex, nullptr, &moles[i]);
        }
    }
    // SDL_Rect molePng {100, 200, 128, 128};
    // SDL_RenderCopy(renderer, testTexture, nullptr, &molePng);
    
    scoreText = "Skor: " + to_string(score);
    textColor = {255, 255, 255, 255};
    textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect.x = 20;
    textRect.y = 20;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    SDL_RenderPresent(renderer);
}

void game::shutdown(){
    if(font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    TTF_Quit();
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
    if (backgroundTex) {
        SDL_DestroyTexture(backgroundTex);
        backgroundTex = nullptr;
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