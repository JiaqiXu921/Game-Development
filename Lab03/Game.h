#pragma once
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "Vehicle.hpp"
#include "Log.hpp"
#include "Frog.hpp"
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
    void AddSprite(class SpriteComponent* spriteToAdd);
    void RemoveSprite(class SpriteComponent* spriteToRemove);
    void AddVehicle(class Vehicle* vehicle);
    void RemoveVehicle(class Vehicle* vehicle);
    void AddLog(class Log* log);
    void RemoveLog(class Log* log);
    SDL_Texture* GetTexture(std::string fileName);
    std::vector<class Vehicle*>& getVehicles() {return mVehicles;}
    std::vector<class Log*>& getLogs() {return mLogs;}
    Actor* getGoal() {return goal;}
    class Frog* myFrog;
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
    int WINDOW_WIDTH=896;
    int WINDOW_HEIGHT=1024;
    int FRAME_LIMITING=16;
    int MAX_DELTATIME=33;
    int TOTAL_ROW=13;
    int BLOCK_SIZE=64;
    int TOP_LEFT_Y=160;
    float LOG_H=48.0f;
    float LOGX_W=192.0f;
    float LOGY_W=256.0f;
    float LOGZ_W=384.0f;
    std::vector<class Vehicle*> mVehicles;
    std::vector<class Log*> mLogs;
    Actor* goal;
};
