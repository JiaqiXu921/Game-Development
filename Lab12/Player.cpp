//
//  Player.cpp
//  Lab07
//
//  Created by Jiaqi Xu on 3/15/21.
//

#include "Player.hpp"
#include "Game.h"
#include "Renderer.h"
#include "PlayerMove.hpp"
#include "CameraComponent.hpp"
#include "CollisionComponent.h"
#include "HUD.hpp"
Player::Player(class Game* game,Actor* parent)
:Actor(game,parent)
,mGame(game)
{
    pm = new PlayerMove(this);
    cc = new CameraComponent(this);
    collisionC = new CollisionComponent(this);
    collisionC->SetSize(50, 175, 50);
    hud = new HUD(this);
}
