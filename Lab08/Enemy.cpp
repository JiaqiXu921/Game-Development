//
//  Enemy.cpp
//  Lab08
//
//  Created by Jiaqi Xu on 3/23/21.
//

#include "Enemy.hpp"
#include "EnemyMove.hpp"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
Enemy::Enemy(class Game* game)
:Actor(game)
{
    SetScale(0.75f);
    mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
    mc->SetTextureIndex(6);
    em = new EnemyMove(this);
}
