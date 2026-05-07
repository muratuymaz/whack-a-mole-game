#include <SDL2/SDL.h>
#include <iostream>
#include "game.hpp"
using namespace std;

int main(int argc, char* argv[]){

    game myGame;

    while (!myGame.init("Pencere",800,600))
    {
        return -1;
    }

    myGame.run();

return 0;
}