//
//  Coin.cpp
//  Lab11
//
//  Created by Jiaqi Xu on 4/12/21.
//

#include "Coin.hpp"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.hpp"
#include "CollisionComponent.h"
#include "HUD.hpp"
Coin::Coin(class Game* game,Actor* parent)
:Actor(game,parent)
{
    mc=new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Coin.gpmesh"));
    cc=new CollisionComponent(this);
    cc->SetSize(100, 100, 100);
}

void Coin::OnUpdate(float deltaTime){
    this->SetRotation(this->GetRotation() + Math::Pi*deltaTime);
    bool intersect = this->cc->Intersect(GetGame()->getPlayer()->getCollision());
    if(intersect){
        Mix_Chunk* coin=GetGame()->GetSound("Assets/Sounds/Coin.wav");
        Mix_PlayChannel(-1, coin, 0);
        GetGame()->getPlayer()->getHUD()->CoinCollected();
        this->SetState(ActorState::Destroy);
    }
}
