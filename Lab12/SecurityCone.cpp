//
//  SecurityCone.cpp
//  Lab11
//
//  Created by Jiaqi Xu on 4/13/21.
//

#include "SecurityCone.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.hpp"
#include <iostream>
SecurityCone::SecurityCone(class Game* game,Actor* parent)
:Actor(game,parent)
{
    mc = new MeshComponent(this,true);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Cone.gpmesh"));
    this->SetPosition(Vector3(30,0,0));
    SetScale(3.0f);
}

void SecurityCone::OnUpdate(float deltaTime){
    Vector3 playerPos = GetGame()->getPlayer()->GetPosition();
    Vector3 conePos = this->GetWorldPosition();
    Vector3 coneFwd = this->GetWorldForward();
    Vector3 playerToCone = playerPos - conePos;
    float angle =Math::Acos(Vector3::Dot(coneFwd, playerToCone)/playerToCone.Length());
    if(Vector3::Dot(playerToCone, coneFwd) < CONE_HEIGHT && angle < HALF_ANGLE){
        mc->SetTextureIndex(1);
        color = "yellow";
        timer -= deltaTime;
        if(timer < 0.0f){
            mc->SetTextureIndex(2);
            color = "red";
            if(timer < -DIE_TIMER){
                GetGame()->getPlayer()->SetPosition(GetGame()->getPlayer()->GetRespawnPos());
            }
        }
    }
    else{
        timer = 2.0f;
        mc->SetTextureIndex(0);
        if(detected!=-1){
            Mix_HaltChannel(detected);
            detected = -1;
        }
        detectPlayOnce = false;
        color = "white";
    }
    if(color == "yellow"){
        if(detectPlayOnce == false){
            Mix_Chunk* detect=GetGame()->GetSound("Assets/Sounds/SecurityDetected.wav");
            detected = Mix_PlayChannel(Mix_GroupAvailable(2), detect, 0);
            detectPlayOnce = true;
        }
    }
}
