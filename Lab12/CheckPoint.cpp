//
//  CheckPoint.cpp
//  Lab11
//
//  Created by Jiaqi Xu on 4/12/21.
//

#include "CheckPoint.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Player.hpp"
#include "Renderer.h"
#include "CollisionComponent.h"
#include <iostream>
CheckPoint::CheckPoint(class Game* game,Actor* parent)
:Actor(game,parent)
{
    mc=new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Checkpoint.gpmesh"));
    cc=new CollisionComponent(this);
    cc->SetSize(25, 25, 25);
}

void CheckPoint::OnUpdate(float deltaTime){
    if(isActive == true){
        mc->SetTextureIndex(0);
    }
    else if(isActive == false){
        mc->SetTextureIndex(1);
    }
}
