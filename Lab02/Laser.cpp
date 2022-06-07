//
//  Laser.cpp
//  Lab02
//
//  Created by Jiaqi Xu on 2/3/21.
//

#include "Laser.hpp"

Laser::Laser(class Game* game)
:Actor(game)
{
    SpriteComponent* sc = new SpriteComponent(this);
    MoveComponent* mc = new MoveComponent(this);
    scL=sc;
    mcL=mc;
    mcL->SetForwardSpeed(FORWARD_SPEED);
    scL->SetTexture(mGame->GetTexture("Assets/Laser.png"));
}

void Laser::OnUpdate(float deltaTime){
    //if the laser was shot over one second, remove it 
    TIME_ALIVE+=deltaTime;
    if(TIME_ALIVE>TIME_BREAK){
        this->SetState(ActorState::Destroy);
    }
    //remove the asteroid and laser if they collide
    for(Asteroid* ast: mGame->mAsteroids){
        Vector2 disVec=ast->GetPosition()-this->GetPosition();
        float dis=disVec.Length();
        if(dis<COLLIDE_DIS){
            ast->SetState(ActorState::Destroy);
            this->SetState(ActorState::Destroy);
        }
    }
}
