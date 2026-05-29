#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>

struct Sprite {
    SDL_Texture* tex = nullptr;
    SDL_Rect dst {0, 0, 0, 0};
    SDL_Rect src {0, 0, 0, 0};
    int zIndex {0};
};

class renderQueue
{
    
public:
    void add(const Sprite& s) {sprites.emplace_back(s);}
    void clear() {sprites.clear();}
    void flush(SDL_Renderer* r) {
        std::sort(sprites.begin(),sprites.end(), [](const Sprite& a, const Sprite& b) {return a.zIndex < b.zIndex;});
        for (const auto& s : sprites) SDL_RenderCopy(r, s.tex, (s.src.w ? &s.src: nullptr), &s.dst);
        sprites.clear();
    }

private:
    std::vector<Sprite> sprites;
};