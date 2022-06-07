#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include "Ghost.h"
#include "PacMan.h"
#include "Random.h"
GhostAI::GhostAI(class Actor* owner)
:Component(owner, 50)
{
    mGhost = static_cast<Ghost*>(owner);
}

void GhostAI::Update(float deltaTime)
{
    //update state time
    if(mState==State::Frightened){
        frightenTimer-=deltaTime;
        if(frightenTimer<0){
            mState=State::Scatter;
        }
    }
    if(mState==State::Scatter){
        scatterTimer-=deltaTime;
        if(scatterTimer<0){
            mState=State::Chase;
            chaseTimer=20.0f;
        }
    }
    if(mState==State::Chase){
        chaseTimer-=deltaTime;
        if(chaseTimer<0){
            mState=State::Scatter;
            scatterTimer=5.0f;
        }
    }

    //update position based on direction
    Vector2 position=mOwner->GetPosition();
    Vector2 positionDiff=mPrevNode->GetPosition()-mNextNode->GetPosition();
    setDirection(positionDiff);
    if(direction=="up"){
        position.y-=ghostSpeed*deltaTime;
    }
    else if(direction=="down"){
        position.y+=ghostSpeed*deltaTime;
    }
    else if(direction=="left"){
        position.x-=ghostSpeed*deltaTime;
    }
    else if(direction=="right"){
        position.x+=ghostSpeed*deltaTime;
    }
    mGhost->SetPosition(position);
    //When reaching nextNode, set the position of the ghost to nextNode's position.
    if(position.x == mNextNode->GetPosition().x && position.y == mNextNode->GetPosition().y){
        mGhost->SetPosition(mNextNode->GetPosition());
    }
    //check if ghost collides with mNextNode
    bool intersect=mGhost->GetComponent<CollisionComponent>()->Intersect(mNextNode->GetComponent<CollisionComponent>());
    if(intersect==true){
        //update path based on state
        if(mState==State::Scatter){
            ghostSpeed=scatterSpeed;
            if(mPath.empty()){
                pathSetUp(mNextNode, mGhost->GetScatterNode());
            }
            else{
                mPrevNode=mNextNode;
                mNextNode=mPath.back();
                mPath.pop_back();
                setDirection(mPrevNode->GetPosition()-mNextNode->GetPosition());
            }
        }
        else if(mState==State::Frightened && !mGhost->IsDead()){
            std::vector<PathNode*> nodesAllowed;
            for(PathNode* adjNode: mNextNode->mAdjacent){
                if(adjNode!=mPrevNode && adjNode->GetType()!=PathNode::Tunnel&&adjNode->GetType()!=PathNode::Ghost){
                    nodesAllowed.push_back(adjNode);
                }
            }
            if(nodesAllowed.empty()){
                for(PathNode* adjNode: mNextNode->mAdjacent){
                    if(adjNode->GetType()==PathNode::Ghost){
                        nodesAllowed.push_back(adjNode);
                    }
                }
            }
            int randNodeNum=Random::GetIntRange(0, nodesAllowed.size()-1);
            mPrevNode=mNextNode;
            mNextNode=nodesAllowed[randNodeNum];
            setDirection(mPrevNode->GetPosition()-mNextNode->GetPosition());
        }
        else if(mState == State::Dead){
            if(!mPath.empty()){
                mPrevNode = mNextNode;
                mNextNode = mPath.back();
                mPath.pop_back();
                setDirection(mPrevNode->GetPosition()-mNextNode->GetPosition());
            }
            else{
                if(mNextNode==mOwner->GetGame()->mGhostPen){
                    mState=State::Scatter;
                    Start(mPrevNode);
                }
                else{
                    pathSetUp(mNextNode, mOwner->GetGame()->mGhostPen);
                }
            }
        }
        else if(mState==State::Chase){
            ghostSpeed=scatterSpeed;
            PathNode* target=getTargetNode();
            pathSetUp(mNextNode, target);
            setDirection(mPrevNode->GetPosition()-mNextNode->GetPosition());
        }
    }
    //update animation
    if(mState!=State::Frightened){
        mGhost->GetComponent<AnimatedSprite>()->SetAnimation(direction);
    }
    else if(mState==State::Frightened){
        if(frightenTimer<=2){
            mGhost->GetComponent<AnimatedSprite>()->SetAnimation("scared1");
        }
        else{
            mGhost->GetComponent<AnimatedSprite>()->SetAnimation("scared0");
        }
    }
   if(mState==State::Dead){
        mGhost->GetComponent<AnimatedSprite>()->SetAnimation("dead"+direction);
    }
}

PathNode* GhostAI::getTargetNode(){
    PathNode* targetNode=nullptr;
    PathNode* pacmanNode=mOwner->GetGame()->mPlayer->GetPrevNode();
    PathNode* defaultNode=mOwner->GetGame()->mPathNodes[0];
    float minTemp=(defaultNode->GetPosition()-pacmanNode->GetPosition()).Length();
    if(mGhost->GetType()==Ghost::Type::Blinky){
        if(pacmanNode->GetType()!=PathNode::Tunnel){
            targetNode = pacmanNode;
        }
        else {
            for(unsigned int i=0;i<mOwner->GetGame()->mPathNodes.size();i++){
                if(mOwner->GetGame()->mPathNodes[i]->GetType()==PathNode::Default){
                    float newLength=(mOwner->GetGame()->mPathNodes[i]->GetPosition()-pacmanNode->GetPosition()).Length();
                    if(newLength<minTemp){
                        defaultNode=mOwner->GetGame()->mPathNodes[i];
                        minTemp=newLength;
                    }
                }
                targetNode=defaultNode;
            }
        }
    }
    else if(mGhost->GetType()==Ghost::Type::Pinky){
        Vector2 pinkyTarget=mOwner->GetGame()->mPlayer->GetPointInFrontOf(80.0f);
        for(unsigned int i=0;i<mOwner->GetGame()->mPathNodes.size();i++){
            if(mOwner->GetGame()->mPathNodes[i]->GetType() == PathNode::Default){
                float newLength=(mOwner->GetGame()->mPathNodes[i]->GetPosition()-pinkyTarget).Length();
                if(newLength<minTemp){
                    defaultNode=mOwner->GetGame()->mPathNodes[i];
                    minTemp=newLength;
                }
            }
        }
        targetNode=defaultNode;
    }
    else if(mGhost->GetType()==Ghost::Type::Inky){
        Vector2 inkyTarget= mOwner->GetGame()->mPlayer->GetPointInFrontOf(40.0f);
        Vector2 v=inkyTarget-mOwner->GetGame()->mGhosts[0]->GetPosition();
        Vector2 q=mOwner->GetGame()->mGhosts[0]->GetPosition()+2*v;
        for(unsigned int i=0;i<mOwner->GetGame()->mPathNodes.size();i++){
            if(mOwner->GetGame()->mPathNodes[i]->GetType() == PathNode::Default){
                float newLength=(mOwner->GetGame()->mPathNodes[i]->GetPosition()-q).Length();
                if(newLength<minTemp){
                    defaultNode=mOwner->GetGame()->mPathNodes[i];
                    minTemp=newLength;
                }
            }
        }
        targetNode=defaultNode;
    }
    else if(mGhost->GetType()==Ghost::Type::Clyde){
        float clydeFromPac=(mOwner->GetGame()->mPlayer->GetPosition()-mGhost->GetPosition()).Length();
        if(clydeFromPac>150.0f&&pacmanNode->GetType()!=PathNode::Tunnel){
            targetNode=pacmanNode;
        }
        else{
            targetNode = mGhost->GetScatterNode();
        }
    }
    return targetNode;
}


void GhostAI::Frighten()
{
    mPath.clear();
    frightenTimer=7.0f;
    scatterTimer=5.0f;
    ghostSpeed=frightenedSpeed;
    mState=State::Frightened;
    PathNode* temp =mNextNode;
    mNextNode = mPrevNode;
    mPrevNode = temp;
   
}

void GhostAI::Start(PathNode* startNode)
{
    scatterTimer=5.0f;
    ghostSpeed=scatterSpeed;
    mOwner->SetPosition(startNode->GetPosition());
    mState=State::Scatter;
    mPrevNode=nullptr;
    mNextNode=nullptr;
    mTargetNode=nullptr;
    pathSetUp(startNode, mGhost->GetScatterNode());
}
void GhostAI::Die()
{
    scatterTimer=5.0f;
    ghostSpeed=deadSpeed;
    mState=State::Dead;
    mPath.clear();
}
void GhostAI::pathSetUp(PathNode* startNode, PathNode* endNode){
    mPath.clear();
    openSet.clear();
    std::unordered_map<PathNode*, NodeInfo> info;
    PathNode* currNode=startNode;
    if(currNode!=endNode){
        info[currNode].IsClosed=true;
    }
    info[currNode].Unusable.push_back(mNextNode);
    info[currNode].Unusable.push_back(mPrevNode);

    do{
        for(PathNode* adjNode: currNode->mAdjacent){
            if(!info[adjNode].IsClosed && adjNode->GetType() != PathNode::Tunnel && std::find(info[currNode].Unusable.begin(), info[currNode].Unusable.end(), adjNode) ==info[currNode].Unusable.end()){
                //if openSet contains adjcenent node
                if(std::find(openSet.begin(), openSet.end(), adjNode) != openSet.end()){
                    float newG=info[currNode].g+getEdgeCost(currNode, adjNode);
                    if(newG<info[adjNode].g){
                        info[adjNode].parent=currNode;
                        info[adjNode].Unusable=info[currNode].Unusable;
                        if(!info[adjNode].Unusable.empty()){
                            info[adjNode].Unusable.pop_back();
                        }
                        info[adjNode].g=newG;
                        info[adjNode].f=info[adjNode].g+info[adjNode].h;
                    }
                }
                else{
                    info[adjNode].parent=currNode;
                    info[adjNode].Unusable=info[currNode].Unusable;
                    if(!info[adjNode].Unusable.empty()){
                        info[adjNode].Unusable.pop_back();
                    }
                    info[adjNode].h=getEdgeCost(adjNode, endNode);
                    info[adjNode].g=info[currNode].g+getEdgeCost(currNode, adjNode);
                    info[adjNode].f=info[adjNode].g+info[adjNode].h;
                    openSet.push_back(adjNode);
                }
            }
        }
        if(openSet.empty()){
            break;
        }
        //find the node with the lowest f in openSet
        float minf=info[openSet[0]].f;
        PathNode* minfNode=openSet[0];
        for(unsigned int i=1;i<openSet.size();i++){
            if(info[openSet[i]].f<minf){
                minf=info[openSet[i]].f;
                minfNode=openSet[i];
            }
        }
        currNode=minfNode;
        openSet.erase(std::find(openSet.begin(), openSet.end(), currNode));
        info[currNode].IsClosed=true;
    }while(currNode!=endNode);
    //update the pathing member data
    if(startNode ==endNode){
        mPath.push_back(currNode);
        currNode = info[currNode].parent;
    }
    while(currNode != startNode){
        mPath.push_back(currNode);
        currNode = info[currNode].parent;
    }
    mTargetNode=endNode;
    mPrevNode=startNode;
    if(!mPath.empty()){
        mNextNode=mPath.back();
        mPath.pop_back();
    }
}

float GhostAI::getEdgeCost(PathNode* currNode, PathNode* adjNode){
    Vector2 vecDiff=currNode->GetPosition()-adjNode->GetPosition();
    return vecDiff.Length();
}

void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}

	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mOwner->GetPosition().x),
			static_cast<int>(mOwner->GetPosition().y),
			static_cast<int>(mNextNode->GetPosition().x),
			static_cast<int>(mNextNode->GetPosition().y));
	}

	// Exit if no path
	if (mPath.empty())
	{
		return;
	}

	if (mNextNode)
	{
		// Line from next node to subsequent on path
		SDL_RenderDrawLine(render,
			static_cast<int>(mNextNode->GetPosition().x),
			static_cast<int>(mNextNode->GetPosition().y),
			static_cast<int>(mPath.back()->GetPosition().x),
			static_cast<int>(mPath.back()->GetPosition().y));
	}

	// Lines for rest of path
	for (size_t i = 0; i < mPath.size() - 1; i++)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mPath[i]->GetPosition().x),
			static_cast<int>(mPath[i]->GetPosition().y),
			static_cast<int>(mPath[i + 1]->GetPosition().x),
			static_cast<int>(mPath[i + 1]->GetPosition().y));
	}
}
//get ghost's facing direction based on position
void GhostAI::setDirection(Vector2 position){
    if(position.x==0 && position.y > 0){
        direction="up";
    }
    else if(position.x==0 && position.y < 0){
        direction="down";
    }
    else if(position.x>0 && position.y==0){
        direction="left";
    }
    else if(position.x<0 && position.y==0){
        direction="right";
    }
}

