#pragma once
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "Math.h"
#include "Player.hpp"
#include "SDL2/SDL_mixer.h" 
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
    void AddBlock(class Block* block);
    void RemoveBlock(class Block* block);
    void AddGoomba(class Goomba* goomba);
    void RemoveGoomba(class Goomba* goomba);
    SDL_Texture* GetTexture(std::string fileName);
    Actor* goal;
    class Player* myPlayer;
    const std::vector<Block*> getBlocks() const;
    std::vector<class Goomba*>& getGoombas() {return mGoombas;}
    Vector2 getCameraPosition() {return cameraPosition;}
    void setCameraPositionX(float xPos) {cameraPosition.x=xPos;}
    Mix_Chunk* GetSound(const std::string& filename);
    int getBackgroundSound() {return backgroundSound;}
private:
    
    void LoadData();
    void UnLoadData();
    SDL_Window* myWindow;
    SDL_Renderer* myRenderer;
    int previousTime=0;
    bool endGame=true;
    std::vector<class Actor*> mActor;
    std::unordered_map<std::string, SDL_Texture*> myMap;
    std::unordered_map<std::string,  Mix_Chunk*> soundMap;
    std::vector<class SpriteComponent*> mSprites;
    int WINDOW_WIDTH=600;
    int WINDOW_HEIGHT=448;
    int FRAME_LIMITING=16;
    int MAX_DELTATIME=33;
    int BROW=16;
    int BCOL=16;
    int BACKGROUND_W=3392;
    int BACKGROUND_H=224;
    int BWIDTH=32;
    int BHEIGHT=32;
    std::vector<class Block*> mBlocks;
    std::vector<class Goomba*> mGoombas;
    Vector2 cameraPosition;
    int backgroundSound;
};
