#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	Mix_Chunk* GetSound(const std::string& fileName);

    class Renderer* GetRenderer()  {	return mRenderer; }
    class Player* getPlayer()  { return player; }
    void AddBlock(class Block* block);
    void RemoveBlock(class Block* block);
    void LoadBlocks(std::string fileName);
    void increaseXGap() { xGap+=1000; }
    const std::vector<class Block*>& getBlocks() const {return blocks;}
    const int getBackgroundSound() const {return backgroundSound;}
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	std::vector<class Actor*> mActors;

	class Renderer* mRenderer = nullptr;

	Uint32 mTicksCount = 0;
	bool mIsRunning;
    
    class Player* player;
    int colorIndex = 1;
    std::vector<class Block*> blocks;
    float blockZ=237.5f;
    float blockY=-237.5f;
    int xGap=1000;
    int backgroundSound;
    float BLOCK_SIZE=25.0f;
    float INIT_Z=237.5f;
    float INIT_Y=-237.5f;
    int GAP_SIZE=1000;
    int COLOR_TOTAL=5;
    float VIEW_WIDTH=1024.0f;
    float VIEW_HEIGHT=768.0f;
    float NEAR_PLANE_DIS=10.0f;
    float FAR_PLANE_DIS=10000.0f;
    float RAD=1.22f;
};
