//
//  Block.cpp
//  Lab07
//
//  Created by Jiaqi Xu on 3/15/21.
//

#include "Block.hpp"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "Game.h"
#include "Player.hpp"
Block::Block(class Game* game)
:Actor(game)
,mGame(game)
{
    this->mScale=25.0f;
    mc = new MeshComponent(this);
    cc = new CollisionComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    mc->SetTextureIndex(2);
    cc->SetSize(1,1,1);
    mGame->AddBlock(this);
}

void Block::OnUpdate(float deltaTime){
    if(mGame->getPlayer()->GetPosition().x - this->GetPosition().x >= dynamicLimit){
        this->SetState(ActorState::Destroy);
    }
}

Block::~Block(){
    mGame->RemoveBlock(this);
}

void Block::chainExplosion(){
    Vector3 centerExplode = this->GetPosition();
    for(unsigned int i = 0;i<mGame->getBlocks().size();i++){
        if(mGame->getBlocks()[i]!=this && mGame->getBlocks()[i]->GetState()!=ActorState::Destroy){
            float distanceDiff = (centerExplode - mGame->getBlocks()[i]->GetPosition()).Length();
            if(distanceDiff<destroyRange){
                mGame->getBlocks()[i]->SetState(ActorState::Destroy);
                if(mGame->getBlocks()[i]->getExplode()==true){
                    mGame->getBlocks()[i]->chainExplosion();
                }
            }
        }
    }
}

