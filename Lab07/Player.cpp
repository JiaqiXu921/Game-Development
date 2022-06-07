//
//  Player.cpp
//  Lab07
//
//  Created by Jiaqi Xu on 3/15/21.
//

#include "Player.hpp"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "PlayerMove.hpp"
Player::Player(class Game* game)
:Actor(game)
,mGame(game)
{
    mc = new MeshComponent(this);
    cc = new CollisionComponent(this);
    pm = new PlayerMove(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Ship.gpmesh"));
    cc->SetSize(100, 40, 60);
}
