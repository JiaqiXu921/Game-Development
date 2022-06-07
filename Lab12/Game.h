#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include <queue>
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

	class Renderer* GetRenderer() const {	return mRenderer; }
    class Player* getPlayer() const { return player; }
    const std::vector<class Actor*>& getBlocks() const {return blocks;}
    void AddBlock(Actor* block);
    void RemoveBlock(Actor* block);
    void AddCamera(class SecurityCamera* camera);
    void RemoveCamera(class SecurityCamera* camera);
    void setPlayer(Player* p) {player = p;}
    std::queue<class CheckPoint*> GetCheckPoints() {return checkPoints;}
    std::queue<class CheckPoint*> checkPoints;
    class Arrow* GetArrow() const {return arrow;}
    void setNextLevel(std::string str) {mNextLevel=str;}
    void LoadNextLevel();
    int totalCoin = 55;
    float time=0.0f;
    bool draw = false;
    bool textUpdated = false;
    class Texture* mCheckPointText;
    const std::vector<class SecurityCamera*>& getCameras() const {return cameras;}
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
    std::vector<class Actor*> blocks;
    std::vector<class SecurityCamera*> cameras;
    float VIEW_WIDTH=1024.0f;
    float VIEW_HEIGHT=768.0f;
    float NEAR_PLANE_DIS=10.0f;
    float FAR_PLANE_DIS=10000.0f;
    float RAD=1.22f;
    class Arrow* arrow;
    std::string mNextLevel="";
};
