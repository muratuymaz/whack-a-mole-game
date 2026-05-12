#pragma once
#include <SDL2/SDL.H>
using namespace std;

struct Button {
    SDL_Texture* tex = nullptr;
    SDL_Texture* textTex = nullptr;
    SDL_Rect rect;
    SDL_Rect textRect;

    // Farenin X ve Y koordinatlarıni kontrol eden fonksiyon
    bool isClicked(int mouseX, int mouseY) {
        if (mouseX >= rect.x && mouseX <= (rect.x + rect.w) &&
            mouseY >= rect.y && mouseY <= (rect.y + rect.h)) {
            return true;
        }
        return false;
    }
};