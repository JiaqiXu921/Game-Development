#pragma once
#include "SDL2/SDL.h"
class Game{
public:
    Game();
    bool Initialize();
    void Shutdown();
    void RunLoop();
    bool ProcessInput();
    void UpdateGame();
    void GenerateOutput();
private:
    SDL_Window* myWindow;
    SDL_Renderer* myRenderer;
    SDL_Point ballV={1,1};
    int thickness=15;
    int windowW=1024;
    int windowH=768;
    SDL_Point paddleC={0,windowH/2};
    int paddleH=200;
    int previousTime=0;
    int direction=0;
    int velocity=500;
    SDL_Point ballC={windowW/2,windowH/2};
    bool endGame=true;
};
