//
//  Block.cpp
//  Lab09
//
//  Created by Jiaqi Xu on 3/26/21.
//

#include "Block.hpp"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"
Block::Block(class Game* game,Actor* parent)
:Actor(game,parent)
,mGame(game)
{
    mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    cc = new CollisionComponent(this);
    cc->SetSize(1,1,1);
    SetScale(64);
    mGame->AddBlock(this);
}

Block::~Block(){
    mGame->RemoveBlock(this);
}

void Block::OnUpdate(float deltaTime){
    if(isRotate){
        this->SetRotation(GetRotation()+deltaTime*ROTATION_ANGLE);
    }
}
