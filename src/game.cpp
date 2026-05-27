#include "game.hpp"

using namespace std;

game::game(){};
game::~game(){
    shutdown();
};

// Oyun dosyalarini tanitiyoruz.
bool game::init(string name, int windowWidth, int windowHeight){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) return false;
    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window) return false;
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) return false;
    SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);
    
    if (TTF_Init() == -1) return false;
    
    font = TTF_OpenFont("assets/fonts/font.ttf", 36);
    if (!font) return false;
    
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return false;

    bgMusic = Mix_LoadMUS("assets/audio/whack-a-mole-bgMusic.mp3");
    if(!bgMusic) return false;
    if(bgMusic) {
        Mix_PlayMusic(bgMusic, -1);
    }

    whackSound = Mix_LoadWAV("assets/audio/bonk.mp3");
    if (!whackSound) return false;

    moleTex = LoadTexture("assets/textures/mole.png");
    if (!moleTex) return false;
    
    moleHitTex = LoadTexture("assets/textures/whacked-mole.png");
    if (!moleHitTex) return false;
    
    backgroundTex = LoadTexture("assets/textures/bg.png");
    if (!backgroundTex) return false;
    
    barTex = LoadTexture("assets/textures/bar.png");
    if (!barTex) return false;
    int barImgW, barImgH;
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

    // High Score table
    highScoreTable = LoadTexture("assets/textures/high-scores-table.png");
    if(!highScoreTable) return false;

    // Game Over table
    gameOverTable = LoadTexture("assets/textures/game-over-table.png");
    if(!gameOverTable) return false;

    // Arkaplan Bluru icin
    overlayTex = LoadTexture("assets/textures/black.png");
    if(!overlayTex) return false;
    SDL_SetTextureAlphaMod(overlayTex, 150);

    // Muzik butonu
    musicButton.tex = LoadTexture("assets/textures/music-button.png");
    if(!musicButton.tex) return false;
    musicButton.rect.w = 80;
    musicButton.rect.h = 80;
    musicButton.rect.x = windowWidth - musicButton.rect.w - 10;
    musicButton.rect.y = windowHeight - musicButton.rect.h - 10;

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
    exitButton.rect.y = (windowHeight / 2) + 140;
    
    // High Score Butonu
    highScoreButton.tex = LoadTexture("assets/textures/button.png");
    highScoreButton.rect.w = 200;
    highScoreButton.rect.h = 80;
    highScoreButton.rect.x = (windowWidth - highScoreButton.rect.w) / 2;
    highScoreButton.rect.y = (windowHeight / 2) + 40;

    SDL_Color btnTextColor = {0, 0, 0, 255};

    // Menu Butonu
    menuButton.tex = LoadTexture("assets/textures/button.png");
    menuButton.rect.w = 200;
    menuButton.rect.h = 80;
    menuButton.rect.x = (windowWidth - menuButton.rect.w) / 2;
    menuButton.rect.y = (windowHeight/2) + 170;
    
    SDL_Surface* menuSurface = TTF_RenderText_Solid(font, "MENU", btnTextColor);
    menuButton.textTex = SDL_CreateTextureFromSurface(renderer, menuSurface);
    menuButton.textRect.w = menuSurface->w;
    menuButton.textRect.h = menuSurface->h;
    menuButton.textRect.x = menuButton.rect.x + (menuButton.rect.w - menuButton.textRect.w) / 2;
    menuButton.textRect.y = menuButton.rect.y + (menuButton.rect.h - menuButton.textRect.h) / 2;
    SDL_FreeSurface(menuSurface);
    
    menuGOButton.tex = LoadTexture("assets/textures/button.png");
    menuGOButton.rect.w = 200;
    menuGOButton.rect.h = 80;
    menuGOButton.rect.x = (windowWidth - menuGOButton.rect.w) / 2;
    menuGOButton.rect.y = (windowHeight/2) + 100;
    
    SDL_Surface* menuGOSurface = TTF_RenderText_Solid(font, "MENU", btnTextColor);
    menuGOButton.textTex = SDL_CreateTextureFromSurface(renderer, menuGOSurface);
    menuGOButton.textRect.w = menuGOSurface->w;
    menuGOButton.textRect.h = menuGOSurface->h;
    menuGOButton.textRect.x = menuGOButton.rect.x + (menuGOButton.rect.w - menuGOButton.textRect.w) / 2;
    menuGOButton.textRect.y = menuGOButton.rect.y + (menuGOButton.rect.h - menuGOButton.textRect.h) / 2;
    SDL_FreeSurface(menuGOSurface);

    // Oyun icindeyken Ana Menu Butonu
    menuPlayingButton.tex = LoadTexture("assets/textures/menu-button.png");
    menuPlayingButton.rect.w = 55;
    menuPlayingButton.rect.h = 55;
    menuPlayingButton.rect.x = 0;
    menuPlayingButton.rect.y = 0;

    // Retry Butonu
    retryButton.tex = LoadTexture("assets/textures/retry-button.png");
    retryButton.rect.w = 200;
    retryButton.rect.h = 80;
    retryButton.rect.x = (windowWidth - retryButton.rect.w) / 2;
    retryButton.rect.y = (windowHeight/2);

    string highScoresText = "HIGH SCORES";
    SDL_Color highScoresTextColor = {0, 0, 0, 255};
    highScoreTextSurface = TTF_RenderText_Solid(font, highScoresText.c_str(), highScoresTextColor);
    highScoreTextTex = SDL_CreateTextureFromSurface(renderer, highScoreTextSurface);
    const int highScoreTextW = highScoreTextSurface->w;
    const int highScoreTextH = highScoreTextSurface->h;
    const int highScoreTextX = (windowWidth - highScoreTextW) / 2;
    SDL_FreeSurface(highScoreTextSurface);
    highScoreTextSurface = nullptr;
    highScoreTextRect.w = highScoreTextW;
    highScoreTextRect.h = highScoreTextH;
    highScoreTextRect.x = highScoreTextX; 
    highScoreTextRect.y = 85;
    
    string gameOverTitleText = "GAME OVER";
    SDL_Color gameOverTextColor = {0, 0, 0, 255};
    gameOverTitleTextSurface = TTF_RenderText_Solid(font, gameOverTitleText.c_str(), gameOverTextColor);
    gameOverTitleTextTex = SDL_CreateTextureFromSurface(renderer, gameOverTitleTextSurface);
    const int gameOverTitleTextW = gameOverTitleTextSurface->w;
    const int gameOverTitleTextH = gameOverTitleTextSurface->h;
    const int gameOverTitleTextX = (windowWidth - gameOverTitleTextW) / 2;
    SDL_FreeSurface(gameOverTitleTextSurface);
    gameOverTitleTextSurface = nullptr;
    gameOverTitleTextRect.w = gameOverTitleTextW;
    gameOverTitleTextRect.h = gameOverTitleTextH;
    gameOverTitleTextRect.x = gameOverTitleTextX; 
    gameOverTitleTextRect.y = 90;

    int gameOverW, gameOverH;
    float scale = 0.8f;
    
    SDL_QueryTexture(gameOverTable, nullptr, nullptr, &gameOverW, &gameOverH);
    
    gameOverRect.w = (int)(gameOverW * scale);
    gameOverRect.h = (int)(gameOverH * scale);
    gameOverRect.x = (windowWidth - gameOverRect.w) / 2;
    gameOverRect.y = 40;

    SDL_Surface* playSurface = TTF_RenderText_Solid(font, "PLAY", btnTextColor);
    playButton.textTex = SDL_CreateTextureFromSurface(renderer, playSurface);
    playButton.textRect.w = playSurface->w;
    playButton.textRect.h = playSurface->h;
    playButton.textRect.x = playButton.rect.x + (playButton.rect.w - playButton.textRect.w) / 2;
    playButton.textRect.y = playButton.rect.y + (playButton.rect.h - playButton.textRect.h) / 2;
    SDL_FreeSurface(playSurface);

    SDL_Surface* exitSurface = TTF_RenderText_Solid(font, "EXIT", btnTextColor);
    exitButton.textTex = SDL_CreateTextureFromSurface(renderer, exitSurface);
    exitButton.textRect.w = exitSurface->w;
    exitButton.textRect.h = exitSurface->h;
    exitButton.textRect.x = exitButton.rect.x + (exitButton.rect.w - exitButton.textRect.w) / 2;
    exitButton.textRect.y = exitButton.rect.y + (exitButton.rect.h - exitButton.textRect.h) / 2;
    SDL_FreeSurface(exitSurface);
    
    SDL_Surface* highScoreSurface = TTF_RenderText_Solid(font, "HIGH SCORES", btnTextColor);
    highScoreButton.textTex = SDL_CreateTextureFromSurface(renderer, highScoreSurface);
    const int maxHighScoreTextW = highScoreButton.rect.w - 24;
    const int maxHighScoreTextH = highScoreButton.rect.h - 18;
    float highScoreScale = 1.0f;
    if (highScoreSurface->w > maxHighScoreTextW || highScoreSurface->h > maxHighScoreTextH) {
        float scaleW = static_cast<float>(maxHighScoreTextW) / static_cast<float>(highScoreSurface->w);
        float scaleH = static_cast<float>(maxHighScoreTextH) / static_cast<float>(highScoreSurface->h);
        highScoreScale = std::min(scaleW, scaleH);
    }
    highScoreButton.textRect.w = static_cast<int>(highScoreSurface->w * highScoreScale);
    highScoreButton.textRect.h = static_cast<int>(highScoreSurface->h * highScoreScale);
    highScoreButton.textRect.x = highScoreButton.rect.x + (highScoreButton.rect.w - highScoreButton.textRect.w) / 2;
    highScoreButton.textRect.y = highScoreButton.rect.y + (highScoreButton.rect.h - highScoreButton.textRect.h) / 2;
    SDL_FreeSurface(highScoreSurface);

    // Baslik
    titleTex = LoadTexture("assets/textures/whack-a-mole-title.png");
    if (!titleTex) return false;
    
    int titleW, titleH;
    SDL_QueryTexture(titleTex, nullptr, nullptr, &titleW, &titleH);
    
    // Baslik boyutu
    float scaleTitle = 0.7f;
    titleRect.w = (int)(titleW * scaleTitle);
    titleRect.h = (int)(titleH * scaleTitle);
    titleRect.x = (windowWidth - titleRect.w) / 2;
    titleRect.y = 40;

    // Bar boyutu
    float scaleBar = 0.38f;
    barRect.w = (int)(barImgW * scaleBar);
    barRect.h = (int)(barImgH* scaleBar);
    barRect.x = (windowWidth - barRect.w) / 2;
    
    int cellSize = 160;
    SDL_Point customPositions[9] = {
        {180, 180}, {350, 160},  {520, 200},
        {130, 300}, {320, 320}, {500,300},
        {160, 430}, {330, 470}, {490, 455}
    };
    
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Hole mole position
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

    loadHighScores();
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
            if(currentState == MENU || currentState == PLAYING || currentState == GAME_OVER) {
                if(musicButton.isClicked(mouseX, mouseY)) {
                    if (Mix_PausedMusic() == 1) { // Muzik durmussa devam et
                        Mix_ResumeMusic();
                    } else { // Muzik caliyorsa durdur
                        Mix_PauseMusic();
                    }
                }
            }
            if (currentState == MENU)
            {
                // Oyna butonuna tiklandiysa:
                if (playButton.isClicked(mouseX, mouseY))
                {
                    currentState = PLAYING;
                    score = 0;
                    timer = 60.0f;
                    isDone = false;
                }
                // Cikis butonuna tiklandiysa:
                else if (exitButton.isClicked(mouseX, mouseY))
                {
                    isRunning = false;
                }
                // High Score butonuna tiklandiysa:
                else if (highScoreButton.isClicked(mouseX, mouseY))
                {
                    currentState = HIGH_SCORE;
                }
            } else if (currentState == HIGH_SCORE)
            {
                if (menuButton.isClicked(mouseX, mouseY))
                {
                    currentState = MENU;
                }
            } else if (currentState == GAME_OVER)
            {
                if (retryButton.isClicked(mouseX, mouseY) || menuGOButton.isClicked(mouseX, mouseY))
                {
                    score = 0;
                    timer = 60.0f;
                    isDone = false;
                    for (int i = 0; i < 9; ++i) {
                        isVisible[i] = false;
                        isHit[i] = false;
                        moleTimers[i] = 0.0f;
                    }
                    if (retryButton.isClicked(mouseX, mouseY)) currentState = PLAYING;
                    if (menuGOButton.isClicked(mouseX, mouseY)) currentState = MENU;
                }
            } else if (currentState == PLAYING) {
                // Oyun icindeyken Menu butonuna tiklandiysa ana menuye don
                if (menuPlayingButton.isClicked(mouseX, mouseY)) {
                    score = 0;
                    timer = 60.0f;
                    isDone = false;
                    for (int i = 0; i < 9; ++i) {
                        isVisible[i] = false;
                        isHit[i] = false;
                        moleTimers[i] = 0.0f;
                    }
                    currentState = MENU;
                    break;
                }
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    SDL_Point mousePoint = { mouseX, mouseY };
                    if (!isDone)
                    {
                        for (int i = 0; i < 9; ++i) {
                            if (!isHit[i] && isVisible[i] && SDL_PointInRect(&mousePoint, &moles[i])) 
                            {
                                cout << "kostebege vurdun!" << endl;
                                isHit[i] = true;
                                moleTimers[i] = 0.0f;
                                score += 10;
                                Mix_PlayChannel(-1, whackSound, 0);
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
        if (timer <= 0.0f && !isDone) {
            timer = 0.0f;
            isDone = true;
            // Yuksek skorlari listeye kaydet
            checkNewHighScore(score);
            currentState = GAME_OVER;
        }
    }
}
// Ekrana cizdirmek icin
void game::render(){
    SDL_RenderClear(renderer);
    vector<SDL_Texture*> tempScoreTextures;

    // Menudeyse menu ekranini cizdir
    if (currentState == MENU) {
        RQ.add({menuBgTex, {0, 0, windowWidth, windowHeight}, {0, 0, 0, 0}, 0});
        RQ.add({overlayTex, {0, 0, windowWidth, windowHeight}, {0, 0, 0, 0}, 1});
        RQ.add({titleTex, titleRect, {0, 0, 0, 0}, 4});
        RQ.add({playButton.tex, playButton.rect, {0, 0, 0, 0}, 5});
        RQ.add({playButton.textTex, playButton.textRect, {0, 0, 0, 0}, 6});
        RQ.add({exitButton.tex, exitButton.rect, {0, 0, 0, 0}, 5});
        RQ.add({exitButton.textTex, exitButton.textRect, {0, 0, 0, 0}, 6});
        RQ.add({highScoreButton.tex, highScoreButton.rect, {0, 0, 0, 0}, 5});
        RQ.add({highScoreButton.textTex, highScoreButton.textRect, {0, 0, 0, 0}, 6});
        musicButton.rect.w = 80;
        musicButton.rect.h = 80;
        musicButton.rect.x = windowWidth - musicButton.rect.w - 10;
        musicButton.rect.y = windowHeight - musicButton.rect.h - 10;
        RQ.add({musicButton.tex, musicButton.rect, {0, 0, 0, 0}, 5});
    } else if (currentState == PLAYING || currentState == GAME_OVER) { /* Oyundaysa oyun ekranini cizdir */
        RQ.add({backgroundTex, {0, 0, windowWidth, windowHeight}, {0, 0, 0, 0}, 0});
        if (currentState == PLAYING)
        {
            RQ.add({barTex, {0, 0, windowWidth, barRect.h}, {0, 0, 0, 0}, 1});
            
            const int barPaddingX = 24;
            const int barPaddingY = 6;
            const int barButtonSpacing = 10;
            const int maxButtonSize = barRect.h - (barPaddingY * 2);
            const int buttonSize = (maxButtonSize < 48) ? maxButtonSize : 48;

            musicButton.rect.w = buttonSize;
            musicButton.rect.h = buttonSize;
            musicButton.rect.x = barPaddingX;
            musicButton.rect.y = barPaddingY + (barRect.h - (barPaddingY * 2) - buttonSize) / 2;
            RQ.add({musicButton.tex, musicButton.rect, {0, 0, 0, 0}, 4});

            // Muzik butonunun yanina Ana Menu butonunu yerlestir
            menuPlayingButton.rect.w = musicButton.rect.w;
            menuPlayingButton.rect.h = musicButton.rect.h;
            menuPlayingButton.rect.x = musicButton.rect.x + musicButton.rect.w + barButtonSpacing;
            menuPlayingButton.rect.y = musicButton.rect.y;
            RQ.add({menuPlayingButton.tex, menuPlayingButton.rect, {0, 0, 0, 0}, 4});
        }

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
        string scoreText = "Score : " + to_string(score);
        SDL_Color textColor = {0, 0, 0, 255};
        textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        textRect.x = (windowWidth - textSurface->w) / 2; 
        textRect.y = 22;

        if(currentState == PLAYING) RQ.add({textTexture, textRect, {0, 0, 0, 0}, 1});
        
        // Timer
        string timerText = "Time : " + to_string((int)(timer));
        SDL_Color timerColor = {0, 0, 0, 255};
        timerSurface = TTF_RenderText_Solid(font, timerText.c_str(), timerColor);
        timerTexture = SDL_CreateTextureFromSurface(renderer, timerSurface);

        timerRect.w = timerSurface->w;
        timerRect.h = timerSurface->h;
        timerRect.x = windowWidth - timerSurface->w - 20; 
        timerRect.y = 22;

        if(currentState == PLAYING) RQ.add({timerTexture, timerRect, {0, 0, 0, 0}, 2});

        if (currentState == GAME_OVER)
        {
            string scoreGOText = "SCORE : " + to_string(score) + "\nWELL DONE!";
            SDL_Color scoreGOTextColor = {0, 0, 0, 255};
            // Alt satira gecirmek icin wrapped fonksiyonu
            TTF_SetFontWrappedAlign(font, TTF_WRAPPED_ALIGN_CENTER);
            scoreGOTextSurface = TTF_RenderText_Blended_Wrapped(font, scoreGOText.c_str(), scoreGOTextColor, 0);
            scoreGOTextTex = SDL_CreateTextureFromSurface(renderer, scoreGOTextSurface);

            scoreGOTextRect.w = scoreGOTextSurface->w;
            scoreGOTextRect.h = scoreGOTextSurface->h;
            scoreGOTextRect.x = (windowWidth - scoreGOTextSurface->w)/2; 
            scoreGOTextRect.y = 180;
            RQ.add({scoreGOTextTex, scoreGOTextRect, {0, 0, 0, 0}, 11});

            // Arkaplan karartmasi
            RQ.add({overlayTex, {0, 0, windowWidth, windowHeight}, {0, 0, 0, 0}, 8});
            RQ.add({gameOverTable, gameOverRect, {0, 0, 0, 0}, 10});
            RQ.add({gameOverTitleTextTex, gameOverTitleTextRect, {0, 0, 0, 0}, 11});

            // Retry Butonu
            RQ.add({retryButton.tex, retryButton.rect, {0, 0, 0, 0}, 12});
            
            // Menu Butonu
            RQ.add({menuGOButton.tex, menuGOButton.rect, {0, 0, 0, 0}, 12});
            RQ.add({menuGOButton.textTex, menuGOButton.textRect, {0, 0, 0, 0}, 13});
            musicButton.rect.w = 80;
            musicButton.rect.h = 80;
            musicButton.rect.x = windowWidth - musicButton.rect.w - 10;
            musicButton.rect.y = windowHeight - musicButton.rect.h - 10;
            RQ.add({musicButton.tex, musicButton.rect, {0, 0, 0, 0}, 14});
        }
        
    } else if (currentState == HIGH_SCORE) { /*High Scoredaysa skorlari cizdir */
        RQ.add({menuBgTex, {0, 0, windowWidth, windowHeight}, {0, 0, 0, 0}, 0});
        RQ.add({overlayTex, {0, 0, windowWidth, windowHeight}, {0, 0, 0, 0}, 1});
        SDL_Color textColor = {255, 255, 0, 255};

        RQ.add({highScoreTextTex,highScoreTextRect,{0, 0, 0, 0,}, 8});

        for (int i = 0; i < highScores.size(); ++i) {
            string text = to_string(i + 1) + ". " + to_string(highScores[i]);
            
            scores = TTF_RenderText_Solid(font, text.c_str(), textColor);
            scoresTex = SDL_CreateTextureFromSurface(renderer, scores);
            
            SDL_Rect highScoresRect;
            highScoresRect.w = scores->w;
            highScoresRect.h = scores->h;
            highScoresRect.x = (windowWidth - highScoresRect.w) / 2;
            highScoresRect.y = 140 + (i * 60);
            
            RQ.add({scoresTex, highScoresRect, {0, 0, 0, 0}, 5});
            tempScoreTextures.push_back(scoresTex);
            
            SDL_FreeSurface(scores);
        }

        int highScoreW, highScoreH;
        float scale = 0.2f;
        
        SDL_QueryTexture(highScoreTable, nullptr, nullptr, &highScoreW, &highScoreH);
        
        highScoreRect.w = (int)(highScoreW * scale);
        highScoreRect.h = (int)(highScoreH * scale);
        highScoreRect.x = (windowWidth - highScoreRect.w) / 2;
        highScoreRect.y = 40;
        
        RQ.add({highScoreTable, {highScoreRect.x, highScoreRect.y, highScoreRect.w, highScoreRect.h}, {0, 0, 0, 0}, 2});

        // Menu Butonu
        RQ.add({menuButton.tex, menuButton.rect, {0, 0, 0, 0}, 5});
        RQ.add({menuButton.textTex, menuButton.textRect, {0, 0, 0, 0}, 6});
    }

    RQ.flush(renderer);

    for(SDL_Texture* tex : tempScoreTextures) SDL_DestroyTexture(tex);
    if (scoreGOTextSurface) {
        SDL_FreeSurface(scoreGOTextSurface);
        scoreGOTextSurface = nullptr;
    }
    if (scoreGOTextTex) {
        SDL_DestroyTexture(scoreGOTextTex);
        scoreGOTextTex = nullptr;
    }

    if (textSurface) {
        SDL_FreeSurface(textSurface);
        textSurface = nullptr;
    }
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
    if (timerSurface) {
        SDL_FreeSurface(timerSurface);
        timerSurface = nullptr;
    }
    if (timerTexture) {
        SDL_DestroyTexture(timerTexture);
        timerTexture = nullptr;
    }
    
    SDL_RenderPresent(renderer);
}
// Acilan seyleri bellek uzerinden temizlemek icin
void game::shutdown(){
    if(font) {
        TTF_CloseFont(font);
        font = nullptr;
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
    if (highScoreTable) {
        SDL_DestroyTexture(highScoreTable);
        highScoreTable = nullptr;
    }
    if (highScoreTextTex) {
        SDL_DestroyTexture(highScoreTextTex);
        highScoreTextTex = nullptr;
    }
    if (gameOverTitleTextTex) {
        SDL_DestroyTexture(gameOverTitleTextTex);
        gameOverTitleTextTex = nullptr;
    }
    if (playButton.tex) {
        SDL_DestroyTexture(playButton.tex);
        playButton.tex = nullptr;
    }
    if (playButton.textTex) {
        SDL_DestroyTexture(playButton.textTex);
        playButton.textTex = nullptr;
    }
    if (exitButton.tex) {
        SDL_DestroyTexture(exitButton.tex);
        exitButton.tex = nullptr;
    }   
    if (exitButton.textTex) {
        SDL_DestroyTexture(exitButton.textTex);
        exitButton.textTex = nullptr;
    }
    if (retryButton.tex) {
        SDL_DestroyTexture(retryButton.tex);
        retryButton.tex = nullptr;
    }
    if (overlayTex) {
        SDL_DestroyTexture(overlayTex);
        overlayTex = nullptr;
    }
    if (gameOverTable) {
        SDL_DestroyTexture(gameOverTable);
        gameOverTable = nullptr;
    }
    if (menuGOButton.tex) {
        SDL_DestroyTexture(menuGOButton.tex);
        menuGOButton.tex = nullptr;
    }
    if (menuGOButton.textTex) { 
        SDL_DestroyTexture(menuGOButton.textTex);
        menuGOButton.textTex = nullptr;
    }
    if (highScoreButton.tex)
    {
        SDL_DestroyTexture(highScoreButton.tex);
        highScoreButton.tex = nullptr;
    }
    if (highScoreButton.textTex)
    {
        SDL_DestroyTexture(highScoreButton.textTex);
        highScoreButton.textTex = nullptr;
    }
    if (menuButton.tex)
    {
        SDL_DestroyTexture(menuButton.tex);
        menuButton.tex = nullptr;
    }
    if (menuButton.textTex)
    {
        SDL_DestroyTexture(menuButton.textTex);
        menuButton.textTex = nullptr;
    }
    if (menuPlayingButton.tex)
    {
        SDL_DestroyTexture(menuPlayingButton.tex);
        menuPlayingButton.tex = nullptr;
    }
    if (titleTex) {
        SDL_DestroyTexture(titleTex);
        titleTex = nullptr;
    }

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
    if(bgMusic) {
        Mix_FreeMusic(bgMusic);
        bgMusic = nullptr;
    }
    if(musicButton.tex) {
        SDL_DestroyTexture(musicButton.tex);
        musicButton.tex = nullptr;
    }
    if (whackSound) {
        Mix_FreeChunk(whackSound);
        whackSound = nullptr;
    }
    
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

// Texture loading
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
// En yuksek skorlari yuklemek icin
void game::loadHighScores() {
    highScores.clear();
    vector<int> allScores;
    
    ifstream file("assets/data/highscores.txt");
    if (file.is_open()) {
        int s;
        while (file >> s) {
            allScores.push_back(s);
        }
        file.close();
    }

    // Siralama
    sort(allScores.begin(), allScores.end(), greater<int>());

    // 5 tane skoru yukle, 5ten azsa 0 koy
    for (int i = 0; i < 5; ++i) {
        if (i < allScores.size()) {
            highScores.push_back(allScores[i]);
        } else {
            highScores.push_back(0); 
        }
    }
}
// High Score kontrolu
void game::checkNewHighScore(int currentScore) {
    // dosyayi sifirlamadan yeni veriyi sona eklemek icin
    ofstream file("assets/data/highscores.txt", ios::app);
    if (file.is_open()) {
        file << currentScore << "\n";
        file.close();
    }
    
    loadHighScores();
}