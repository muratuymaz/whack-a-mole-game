#pragma once

#include <SDL2/SDL.H>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "renderQueue.h"
#include "button.h"

struct TTF_Font;

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
    void loadHighScores();
    void checkNewHighScore(int score);
    
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* LoadTexture(const string& path);

    SDL_Texture* moleTex= nullptr;
    SDL_Texture* moleHitTex= nullptr;
    SDL_Texture* backgroundTex = nullptr;
    SDL_Texture* barTex = nullptr;
    SDL_Surface* timerSurface = nullptr;
    SDL_Texture* timerTexture = nullptr;
    SDL_Surface* textSurface = nullptr;
    SDL_Texture* textTexture = nullptr;
    SDL_Texture* gameOverTex = nullptr;
    SDL_Surface* gameOverSurface = nullptr;
    SDL_Rect timerRect;
    SDL_Rect textRect;
    SDL_Rect barRect;
    SDL_Rect gameOverTextRect;

    renderQueue RQ;

    SDL_Rect holes[9];
    SDL_Rect moles[9];
    SDL_Rect hitMoles[9];
    SDL_Texture* holeTex = nullptr;
    bool isVisible[9] = {false};
    bool isHit[9] = {false};

    float spawnTimer = 0.0f;
    float currentSpawnDelay = 1.0f;
    float moleTimers[9] = {0.0f};

    int score = 0;
    SDL_Rect scoreGOTextRect;
    SDL_Surface* scoreGOTextSurface = nullptr;
    SDL_Texture* scoreGOTextTex = nullptr;
    TTF_Font* font = nullptr;

    float timer = 60.0;
    bool isDone = false;

    enum GameState { MENU, PLAYING, GAME_OVER, HIGH_SCORE};
    GameState currentState;

    SDL_Texture* menuBgTex = nullptr;
    SDL_Texture* highScoreTable = nullptr;
    SDL_Rect highScoreRect;
    SDL_Texture* highScoreTextTex = nullptr;
    SDL_Surface* highScoreTextSurface = nullptr;
    SDL_Rect highScoreTextRect;
    SDL_Texture* gameOverTitleTextTex = nullptr;
    SDL_Surface* gameOverTitleTextSurface = nullptr;
    SDL_Rect gameOverTitleTextRect;
    SDL_Texture* gameOverTable = nullptr;
    SDL_Rect gameOverRect;
    SDL_Texture* overlayTex = nullptr;
    Button playButton;
    Button exitButton;
    Button highScoreButton;
    Button menuButton;
    Button menuGOButton;
    Button retryButton;
    Button musicButton;
    bool isMusicRunning;
    Mix_Music* bgMusic = nullptr;
    Mix_Chunk* whackSound = nullptr;

    bool isRunning = false;
    bool isFullscreen = false;
    int windowWidth = 800;
    int windowHeight = 600;

    SDL_Texture* titleTex = nullptr;
    SDL_Rect titleRect;

    const string highScorePath = "assets/data/highscore.txt";
    vector<int> highScores;
    SDL_Surface* scores = nullptr;
    SDL_Texture* scoresTex = nullptr;
};