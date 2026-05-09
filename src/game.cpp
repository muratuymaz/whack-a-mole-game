#include "game.hpp"

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
    SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);
    
    if (TTF_Init() == -1) return false;

    font = TTF_OpenFont("assets/fonts/font.ttf", 36);
    if (!font) return false;

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    
    moleTex = LoadTexture("assets/textures/mole.png");
    if (!moleTex) return false;

    backgroundTex = LoadTexture("assets/textures/bg.png");
    if (!backgroundTex) return false;
    
    holeTex = LoadTexture("assets/textures/hole.png");
    if (!holeTex) return false;

    int holeImgW, holeImgH;
    SDL_QueryTexture(holeTex, nullptr, nullptr, &holeImgW, &holeImgH);
    
    int moleImgW, moleImgH;
    SDL_QueryTexture(moleTex, nullptr, nullptr, &moleImgW, &moleImgH);
    
    int cellSize = 160;

    SDL_Point customPositions[9] = {
        {180, 180}, {350, 160},  {520, 200},
        {130, 300}, {320, 320}, {500,300},
        {160, 430}, {330, 470}, {490, 455}
    };

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = 0; i < 9; ++i) {
        holes[i].w = cellSize;
        holes[i].h = (cellSize * holeImgH) / holeImgW;
        
        holes[i].x = customPositions[i].x;
        holes[i].y = customPositions[i].y;

        moles[i].w = static_cast<int>(cellSize * 0.35); 
        moles[i].h = (moles[i].w * moleImgH) / moleImgW; 
        
        moles[i].x = holes[i].x + (holes[i].w - moles[i].w) / 2;
        moles[i].y = holes[i].y + (holes[i].h - moles[i].h) / 2 -52;

        isVisible[i] = false; 
        moleTimers[i] = 0.0f;
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
                break;
            }
            if (event.key.keysym.sym == SDLK_F11) {
                isFullscreen = !isFullscreen;
                if (isFullscreen) {
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                } else SDL_SetWindowFullscreen(window, 0); 
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                SDL_Point mousePoint = { mouseX, mouseY };
                static int index = 1;
                if (!isDone)
                {
                    for (int i = 0; i < 9; ++i) {
                        if (isVisible[i] && SDL_PointInRect(&mousePoint, &moles[i])) 
                        {
                            cout << index << ". kostebege vurdun!" << endl;
                            isVisible[i] = false;
    
                            index++;
                            score += 10;
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
    spawnTimer += dt;
    if (!isDone)
    {
        timer -= dt;
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
    if (timer <= 0) {
        timer = 0;
        isDone = true;
    }
}

void game::render(){
    SDL_RenderClear(renderer);
    RQ.add({backgroundTex, {0, 0, windowWidth, windowHeight}, {0, 0, 0, 0}, 0});

    for (int i = 0; i < 9; ++i) {
        RQ.add({holeTex, holes[i], {0, 0, 0, 0}, 1}); 
    }

    for (int i = 0; i < 9; ++i) {
        if (isVisible[i]) {
            RQ.add({moleTex, moles[i], {0, 0, 0, 0}, 2});
        }
    }

    string scoreText = "Skor:" + to_string(score);
    SDL_Color textColor = {0, 0, 0, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    
    SDL_Rect textRect; 
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    textRect.x = (windowWidth - textSurface->w) / 2; 
    textRect.y = 20;

    RQ.add({textTexture, textRect, {0, 0, 0, 0}, 10});

    string timerText = "Time:" + to_string((int)(timer));
    SDL_Color timerColor = {0, 0, 0, 255};
    SDL_Surface* timerSurface = TTF_RenderText_Solid(font, timerText.c_str(), timerColor);
    SDL_Texture* timerTexture = SDL_CreateTextureFromSurface(renderer, timerSurface);
    
    SDL_Rect timerRect;
    timerRect.w = timerSurface->w;
    timerRect.h = timerSurface->h;
    timerRect.x = windowWidth - timerSurface->w - 20; 
    timerRect.y = 20;

    RQ.add({timerTexture, timerRect, {0, 0, 0, 0}, 10});

    RQ.flush(renderer);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(timerSurface);
    SDL_DestroyTexture(timerTexture);

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
    if (holeTex) {
        SDL_DestroyTexture(holeTex);
        holeTex = nullptr;
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