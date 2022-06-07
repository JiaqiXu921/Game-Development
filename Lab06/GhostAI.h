#pragma once
#include "Component.h"
#include "Math.h"
#include <vector>
#include <string>
struct NodeInfo {
    class PathNode* parent=nullptr;
    float f=0.0f;
    float g=0.0f;
    float h=0.0f;
    bool IsClosed=false;
    std::vector<PathNode*> Unusable;
};
class GhostAI : public Component
{
public:
    
	// Used to track the four different GhostAI states
	enum State
	{
		Scatter,
		Chase,
		Frightened,
		Dead
	};
	
	GhostAI(class Actor* owner);

	void Update(float deltaTime) override;
	
	// Called when the Ghost starts at the beginning
	// (or when the ghosts should respawn)
	void Start(class PathNode* startNode);
	
	// Get the current state
	State GetState() const { return mState; }
	
	// Called when the ghost should switch to the "Frightened" state
	void Frighten();
	
	// Called when the ghost should switch to the "Dead" state
	void Die();

	//  Helper function to draw GhostAI's current path
	void DebugDrawPath(struct SDL_Renderer* render);
    void pathSetUp(PathNode* startNode, PathNode* endNode);
    float getEdgeCost(PathNode* currNode, PathNode* adjNode);
    void setDirection(Vector2 position);
    void minCalcualtor(float minTemp, PathNode* nodeI);
    PathNode* getTargetNode();
private:
    
	// Member data for pathfinding

	// TargetNode is our current goal node
	class PathNode* mTargetNode = nullptr;
	// PrevNode is the last node we intersected
	// with prior to the current position
	class PathNode* mPrevNode = nullptr;
	// NextNode is the next node we're trying
	// to get to
	class PathNode* mNextNode = nullptr;

	// This vector always contains the path
	// from "next node" to "target node"
	// (if there is still such a path)
	std::vector<class PathNode*> mPath;

	// Current state of the Ghost AI
	State mState = Scatter;

	// Save the owning actor (cast to a Ghost*)
	class Ghost* mGhost;
    
	// TODO: Add any member data/helper functions here!
    std::vector<class PathNode*> openSet;
    std::string direction="";
    float scatterSpeed=90.0f;
    float frightenedSpeed=65.0f;
    float deadSpeed=125.0f;
    float ghostSpeed=90.0f;
    float frightenTimer=7.0f;
    float scatterTimer=5.0f;
    float chaseTimer=20.0f;
};
