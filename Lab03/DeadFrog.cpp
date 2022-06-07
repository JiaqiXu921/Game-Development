//
//  DeadFrog.cpp
//  Lab03
//
//  Created by Jiaqi Xu on 2/9/21.
//

#include "DeadFrog.hpp"
DeadFrog::DeadFrog(class Game* game)
:Actor(game)
{
    SpriteComponent* sC = new SpriteComponent(this);
    scDead=sC;
    scDead->SetTexture(mGame->GetTexture("Assets/Dead.png"));
    
}

void DeadFrog::OnUpdate(float deltaTime){
    //remove deadfrog image if it stays over 0.5 secs
    timeAlive+=deltaTime;
    if(timeAlive>0.5){
        this->SetState(ActorState::Destroy);
    }
}
