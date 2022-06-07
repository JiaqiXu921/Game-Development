//
//  Arrow.cpp
//  Lab11
//
//  Created by Jiaqi Xu on 4/12/21.
//

#include "Arrow.hpp"
#include "Game.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "CheckPoint.hpp"
#include "Player.hpp"
#include "PlayerMove.hpp"
#include <iostream>
Arrow::Arrow(class Game* game,Actor* parent)
:Actor(game,parent)
{
    mc=new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Arrow.gpmesh"));
    SetScale(0.15f);
}

void Arrow::OnUpdate(float deltaTime){
    Quaternion q;
    CheckPoint* activePoint = this->GetGame()->getPlayer()->getPM()->GetNextCheckPoint();
        Vector3 playerToActive =activePoint->GetPosition()-this->GetGame()->getPlayer()->GetPosition();
        playerToActive.Normalize();
        Vector3 unit = Vector3(1,0,0);
        float dot = Vector3::Dot(unit, playerToActive);
        if(dot==1){
            q=Quaternion::Identity;
        }
        else if(dot==-1){
            q=Quaternion(Vector3::UnitZ, Math::Pi);
        }
        else if(activePoint == nullptr){
            q=Quaternion::Identity;
        }
        else{
            float theta = Math::Acos(dot);
            Vector3 axis = Vector3::Cross(unit, playerToActive);
            axis.Normalize();
            q=Quaternion(axis,theta);
        }
    GetGame()->GetArrow()->SetQuaternion(q);

    Vector3 worldPos = this->GetGame()->GetRenderer()->Unproject(Vector3(0.0f, 250.0f, 0.1f));
    this->SetPosition(worldPos);
}
