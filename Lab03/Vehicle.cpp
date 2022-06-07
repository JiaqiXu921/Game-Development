//
//  Vehicle.cpp
//  itp380labs
//
//  Created by Jiaqi Xu on 2/5/21.
//

#include "Vehicle.hpp"
#include "WrappingMove.hpp"
#include "CollisionComponent.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
Vehicle::Vehicle(class Game* game)
:Actor(game)
{
    SpriteComponent* sC = new SpriteComponent(this);
    sc=sC;
    WrappingMove* wmL=new WrappingMove(this);
    wm=wmL;
    wm->SetForwardSpeed(FORWARD_SPEED);
    CollisionComponent* cC= new CollisionComponent(this);
    cc=cC;
    if(this->rowNum==7){
        cc->SetSize(TRUCK_W, TRUCK_H);
    }
    else{
        cc->SetSize(CAR_WH, CAR_WH);
    }
    
    mGame->AddVehicle(this);
}


Vehicle::~Vehicle(){
    mGame->RemoveVehicle(this);
}

void Vehicle::OnUpdate(float deltaTime){
    float vehicleX=wm->direction.x;
    Vector2 vehicleDir(vehicleX,0);
    Vector2 frogDir=mGame->myFrog->GetPosition();
    Vector2 relPos=frogDir-this->GetPosition();
    float dotProduct=Vector2::Dot(vehicleDir, relPos);
    float angle=Math::Acos(dotProduct/(vehicleDir.Length()*relPos.Length()));
    
    if(angle<abs(M_PI/6)){
        wm->SetForwardSpeed(50.f);
    }
    else{
        wm->SetForwardSpeed(100.f);
    }
}
