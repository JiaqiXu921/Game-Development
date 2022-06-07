//
//  LaserMine.cpp
//  Lab10
//
//  Created by Jiaqi Xu on 4/5/21.
//

#include "LaserMine.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "LaserComponent.hpp"
LaserMine::LaserMine(class Game* game, Actor* parent)
:Actor(game,parent)
,mGame(game)
{
    mc = new MeshComponent(this);
    mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/LaserMine.gpmesh"));
    lc=new LaserComponent(this);
}
