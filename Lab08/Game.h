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

	class Renderer* GetRenderer() {	return mRenderer; }
    class Player* getPlayer() { return player; }
    void increaseXGap() { xGap+=1000; }
    std::vector<class Block*>& getBlocks() {return blocks;}
    int getBackgroundSound() {return backgroundSound;}
    class HeightMap* getHeightMap() {return hm;}
    class Enemy* getEnemy() {return enemy;}
    Mix_Chunk* getFinalLap() {return finalLap;}
    Mix_Chunk* getWon() {return won;}
    Mix_Chunk* getLost() {return lost;}
    Mix_Chunk* getMusicFast() {return musicFast;}
    int playing = 0;
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
    int xGap=1000;
    int backgroundSound;
    int GAP_SIZE=1000;
    int COLOR_TOTAL=5;
    float VIEW_WIDTH=1024.0f;
    float VIEW_HEIGHT=768.0f;
    float NEAR_PLANE_DIS=10.0f;
    float FAR_PLANE_DIS=10000.0f;
    float RAD=1.22f;
    class HeightMap* hm;
    class Enemy* enemy;
    float iniTime = 8.5f;
    bool once = true;
    Mix_Chunk* finalLap;
    Mix_Chunk* lost;
    Mix_Chunk* music;
    Mix_Chunk* raceStart;
    Mix_Chunk* won;
    Mix_Chunk* musicFast;
};
