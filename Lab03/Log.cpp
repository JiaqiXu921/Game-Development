//
//  Log.cpp
//  Lab03
//
//  Created by Jiaqi Xu on 2/5/21.
//

#include "Log.hpp"
#include "SpriteComponent.h"
Log::Log(class Game* game)
:Actor(game)
{
    SpriteComponent* sC = new SpriteComponent(this);
    sc=sC;
    WrappingMove* wmL=new WrappingMove(this);
    wm=wmL;
    wm->SetForwardSpeed(FORWARD_SPEED);
    
    CollisionComponent* cC= new CollisionComponent(this);
    cc=cC;
    mGame->AddLog(this);
}
Log::~Log(){
    mGame->RemoveLog(this);
}
