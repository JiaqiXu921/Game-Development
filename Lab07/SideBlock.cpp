//
//  SideBlock.cpp
//  Lab07
//
//  Created by Jiaqi Xu on 3/15/21.
//

#include "SideBlock.hpp"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.hpp"
SideBlock::SideBlock(class Game* game)
:Actor(game)
,mGame(game)
{
    this->mScale=500;
    mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
    mc->SetTextureIndex(1);
}

void SideBlock::OnUpdate(float deltaTime){
    if(mGame->getPlayer()->GetPosition().x - this->GetPosition().x >= 2000){
        this->SetState(ActorState::Destroy);
    }
}
