#include "game.hpp"

using namespace std;

game::game(){};
game::~game(){
    shutdown();
};

// Oyun dosyalarini tanitiyoruz.
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
    
    moleHitTex = LoadTexture("assets/textures/whacked-mole.png");
    if (!moleHitTex) return false;
    
    backgroundTex = LoadTexture("assets/textures/bg.png");
    if (!backgroundTex) return false;
    
    barTex = LoadTexture("assets/textures/bar.png");
    if (!barTex) return false;
    SDL_QueryTexture(barTex, nullptr, nullptr, &barImgW, &barImgH);
    
    holeTex = LoadTexture("assets/textures/hole.png");
    if (!holeTex) return false;
    
    // Hole
    int holeImgW, holeImgH;
    SDL_QueryTexture(holeTex, nullptr, nullptr, &holeImgW, &holeImgH);
    
    // Mole
    int moleImgW, moleImgH;
    SDL_QueryTexture(moleTex, nullptr, nullptr, &moleImgW, &moleImgH);
    
    // Whacked Mole
    int moleHitImgW, moleHitImgH;
    SDL_QueryTexture(moleHitTex, nullptr, nullptr, &moleHitImgW, &moleHitImgH);

    // Menu arkaplani
    menuBgTex = LoadTexture("assets/textures/menu.png");
    if (!menuBgTex) return false;

    // Oyna Butonu
    playButton.tex = LoadTexture("assets/textures/button.png");
    playButton.rect.w = 200;
    playButton.rect.h = 80;
    playButton.rect.x = (windowWidth - playButton.rect.w) / 2;
    playButton.rect.y = (windowHeight / 2) - 60;

    // Cikis Butonu
    exitButton.tex = LoadTexture("assets/textures/button.png");
    exitButton.rect.w = 200;
    exitButton.rect.h = 80;
    exitButton.rect.x = (windowWidth - exitButton.rect.w) / 2;
    exitButton.rect.y = (windowHeight / 2) + 40;

    SDL_Color btnTextColor = {0, 0, 0, 255}; 

    SDL_Surface* playSurface = TTF_RenderText_Solid(font, "OYNA", btnTextColor);
    playButton.textTex = SDL_CreateTextureFromSurface(renderer, playSurface);
    playButton.textRect.w = playSurface->w;
    playButton.textRect.h = playSurface->h;
    playButton.textRect.x = playButton.rect.x + (playButton.rect.w - playButton.textRect.w) / 2;
    playButton.textRect.y = playButton.rect.y + (playButton.rect.h - playButton.textRect.h) / 2;
    SDL_FreeSurface(playSurface);

    SDL_Surface* exitSurface = TTF_RenderText_Solid(font, "CIKIS", btnTextColor);
    exitButton.textTex = SDL_CreateTextureFromSurface(renderer, exitSurface);
    exitButton.textRect.w = exitSurface->w;
    exitButton.textRect.h = exitSurface->h;
    exitButton.textRect.x = exitButton.rect.x + (exitButton.rect.w - exitButton.textRect.w) / 2;
    exitButton.textRect.y = exitButton.rect.y + (exitButton.rect.h - exitButton.textRect.h) / 2;
    SDL_FreeSurface(exitSurface);

    // Baslik
    titleTex = LoadTexture("assets/textures/title1.png");
    if (!titleTex) return false;
    
    int titleW, titleH;
    SDL_QueryTexture(titleTex, nullptr, nullptr, &titleW, &titleH);
    
    float scale = 0.5f;
    titleRect.w = (int)(titleW * scale);
    titleRect.h = (int)(titleH * scale);
    titleRect.x = (windowWidth - titleRect.w) / 2;
    titleRect.y = 40;
    
    int cellSize = 160;
    SDL_Point customPositions[9] = {
        {180, 180}, {350, 160},  {520, 200},
        {130, 300}, {320, 320}, {500,300},
        {160, 430}, {330, 470}, {490, 455}
    };
    
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Hole position
    for (int i = 0; i < 9; ++i) {
        holes[i].w = cellSize;
        holes[i].h = (cellSize * holeImgH) / holeImgW;
        
        holes[i].x = customPositions[i].x;
        holes[i].y = customPositions[i].y;
        
        moles[i].w = static_cast<int>(cellSize * 0.35); 
        moles[i].h = (moles[i].w * moleImgH) / moleImgW; 
        
        moles[i].x = holes[i].x + (holes[i].w - moles[i].w) / 2;
        moles[i].y = holes[i].y + (holes[i].h - moles[i].h) / 2 -52;
        
        hitMoles[i].w = static_cast<int>(cellSize * 0.47); 
        hitMoles[i].h = (hitMoles[i].w * moleHitImgH) / moleHitImgW; 
        
        hitMoles[i].x = holes[i].x + (holes[i].w - hitMoles[i].w) / 2;
        hitMoles[i].y = holes[i].y + (holes[i].h - hitMoles[i].h) / 2 -52;
        
        isVisible[i] = false;
        isHit[i] = false;
        moleTimers[i] = 0.0f;
    }
    
    currentState = MENU;
    isRunning = true;
    return true;
}
void game::run(){
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 freq = SDL_GetPerformanceFrequency();

    // Game loop
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
    // Mouse and keyboard inputs
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
        {
            int mouseX = event.button.x;
            int mouseY = event.button.y;
            
            if (currentState == MENU)
            {
                // Oyna butonuna tiklandiysa:
                if (playButton.isClicked(mouseX, mouseY))
                {
                    currentState = PLAYING;
                    score = 0;
                    timer = 15.0f;
                    isDone = false;
                }
                // Cikis butonuna tiklandiysa:
                else if (exitButton.isClicked(mouseX, mouseY))
                {
                    isRunning = false;
                }
            } else if (currentState == PLAYING)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    SDL_Point mousePoint = { mouseX, mouseY };
                    static int index = 1;
                    if (!isDone)
                    {
                        for (int i = 0; i < 9; ++i) {
                            if (!isHit[i] && isVisible[i] && SDL_PointInRect(&mousePoint, &moles[i])) 
                            {
                                cout << index << ". kostebege vurdun!" << endl;
                                isHit[i] = true;
                                moleTimers[i] = 0.0f;
        
                                index++;
                                score += 10;
                            }
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
    if (currentState == PLAYING)
    {
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
                    if (!isHit[i])
                    {
                        if (moleTimers[i] >= 1.5f) {
                        isVisible[i] = false;
                        }
                    }else {
                        if (moleTimers[i] >= 1.0f) {
                            isVisible[i] = false;
                            isHit[i] = false;
                        }
                    }
                }
            }
        }
        if (timer <= 0) {
            timer = 0;
            isDone = true;
        }
    }
}

void game::render(){
    SDL_RenderClear(renderer);
    RQ.add({backgroundTex, {0, 0, windowWidth, windowHeight}, {0, 0, 0, 0}, 0});
    // RQ.add({barTex, {0, 0, 2444, 225}, {0, 0, 0, 0}, 0}); DUZENLENECEK !!!!
    
    // Menudeyse menu ekranini cizdir
    if (currentState == MENU) {
        RQ.add({menuBgTex, {0, 0, windowWidth, windowHeight}, {0, 0, 0, 0}, 0});
        RQ.add({titleTex, titleRect, {0, 0, 0, 0}, 4});
        RQ.add({playButton.tex, playButton.rect, {0, 0, 0, 0}, 5});
        RQ.add({playButton.textTex, playButton.textRect, {0, 0, 0, 0}, 6});
        RQ.add({exitButton.tex, exitButton.rect, {0, 0, 0, 0}, 5});
        RQ.add({exitButton.textTex, exitButton.textRect, {0, 0, 0, 0}, 6});

    } else if (currentState == PLAYING) { /* Oyundaysa oyun ekranini cizdir */
        RQ.add({backgroundTex, {0, 0, windowWidth, windowHeight}, {0, 0, 0, 0}, 0});

        for (int i = 0; i < 9; ++i) {
            RQ.add({holeTex, holes[i], {0, 0, 0, 0}, 1}); 
        }

        for (int i = 0; i < 9; ++i) {
            if (isVisible[i]) {
                if (!isHit[i])
                {
                    RQ.add({moleTex, moles[i], {0, 0, 0, 0}, 2});
                }else {
                    RQ.add({moleHitTex, hitMoles[i], {0, 0, 0, 0}, 2});
                }
            }
        }
        // Scoreboard
        string scoreText = "Skor:" + to_string(score);
        SDL_Color textColor = {0, 0, 0, 255};
        textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        textRect.x = (windowWidth - textSurface->w) / 2; 
        textRect.y = 20;

        RQ.add({textTexture, textRect, {0, 0, 0, 0}, 1});
        // Timer
        string timerText = "Time:" + to_string((int)(timer));
        SDL_Color timerColor = {0, 0, 0, 255};
        timerSurface = TTF_RenderText_Solid(font, timerText.c_str(), timerColor);
        timerTexture = SDL_CreateTextureFromSurface(renderer, timerSurface);

        timerRect;
        timerRect.w = timerSurface->w;
        timerRect.h = timerSurface->h;
        timerRect.x = windowWidth - timerSurface->w - 20; 
        timerRect.y = 20;

        RQ.add({timerTexture, timerRect, {0, 0, 0, 0}, 2});
    }

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
    if (titleTex) {
        SDL_DestroyTexture(titleTex);
        titleTex = nullptr;
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
    if (moleTex) {
        SDL_DestroyTexture(moleTex);
        moleTex = nullptr;
    }
    if (moleHitTex) {
        SDL_DestroyTexture(moleHitTex);
        moleHitTex = nullptr;
    }
    if (barTex) {
        SDL_DestroyTexture(barTex);
        barTex = nullptr;
    }
    if (menuBgTex) {
        SDL_DestroyTexture(menuBgTex);
        menuBgTex = nullptr;
    }
    if (playButton.tex) {
        SDL_DestroyTexture(playButton.tex);
        playButton.tex = nullptr;
    }
    if (exitButton.tex) {
        SDL_DestroyTexture(exitButton.tex);
        exitButton.tex = nullptr;
    }
    if (playButton.textTex) {
        SDL_DestroyTexture(playButton.textTex);
    }
    if (exitButton.textTex) {
        SDL_DestroyTexture(exitButton.textTex);
    }
    
    SDL_Quit();
}

SDL_Texture* game::LoadTexture(const string& path){
    // Texture loading
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