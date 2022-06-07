//
//  Bullet.cpp
//  Lab07
//
//  Created by Jiaqi Xu on 3/16/21.
//

#include "Bullet.hpp"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "MoveComponent.h"
#include "Block.hpp"
#include "Player.hpp"
#include "PlayerMove.hpp"
Bullet::Bullet(class Game* game)
:Actor(game)
,mGame(game)
{
    this->mScale=0.5f;
    mc = new MeshComponent(this);
    cc = new CollisionComponent(this);
    move=new MoveComponent(this);
    move->SetForwardSpeed(500.0f+defaultSpeed*mGame->getPlayer()->getPM()->getTimeMultiplier());
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"));
    cc->SetSize(10, 10, 10);
}

void Bullet::OnUpdate(float deltaTime){
    TIME_ALIVE+=deltaTime;
    if(TIME_ALIVE>1){
        this->SetState(ActorState::Destroy);
    }
    for(Block* block:mGame->getBlocks()){
        bool intersect = cc->Intersect(block->getCC());
        if(intersect){
            if(block->getExplode()==false){
                this->SetState(ActorState::Destroy);
            }
            //chain reaction
            else if(block->getExplode()==true){
                block->chainExplosion();
                this->SetState(ActorState::Destroy);
            }
        }
    }
    
    //explosion sound only plays once
    for(Block* block:mGame->getBlocks()){
        bool intersect = cc->Intersect(block->getCC());
        if(intersect){
            if(block->getExplode()==true){
                Mix_Chunk* explode=mGame->GetSound("Assets/Sounds/BlockExplode.wav");
                Mix_PlayChannel(-1, explode, 0);
                break;
            }
        }
    }
}
