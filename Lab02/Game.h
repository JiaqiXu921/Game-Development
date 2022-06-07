#pragma once
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include <unordered_map>
class Game{
public:
    Game();
    bool Initialize();
    void Shutdown();
    void RunLoop();
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);
    void AddAsteroid(class Asteroid* asteroid);
    void RemoveAsteroid(class Asteroid* asteroid);
    void AddSprite(class SpriteComponent* spriteToAdd);
    void RemoveSprite(class SpriteComponent* spriteToRemove);
    SDL_Texture* GetTexture(std::string fileName);
    //getter of asteroid
    std::vector<class Asteroid*> mAsteroids;
private:
    void LoadData();
    void UnLoadData();
    SDL_Window* myWindow;
    SDL_Renderer* myRenderer;
    int previousTime=0;
    bool endGame=true;
    std::vector<class Actor*> mActor;
    std::unordered_map<std::string, SDL_Texture*> myMap;
    std::vector<class SpriteComponent*> mSprites;
    int FRAME_LIMITING=16;
    int MAX_DELTATIME=33;
    int NUM_ASTEROID=10;
    int WINDOW_WIDTH=1024;
    int WINDOW_HEIGHT=768;
    int UPDATE_ORDER=90;
};
