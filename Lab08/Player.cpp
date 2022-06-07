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
#include "PlayerMove.hpp"
#include "CameraComponent.hpp"
#include "PlayerUI.h"
Player::Player(class Game* game)
:Actor(game)
,mGame(game)
{
    SetScale(0.75f);
    mc = new MeshComponent(this);
    pm = new PlayerMove(this);
    cc = new CameraComponent(this);
    cc->SnapToIdeal();
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
    ui = new PlayerUI(this);
}

