//
//  Ship.cpp
//  Lab02
//
//  Created by Jiaqi Xu on 2/2/21.
//

#include "Ship.hpp"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Laser.hpp"
Ship::Ship(class Game* game)
:Actor(game)
{
    SpriteComponent* sC = new SpriteComponent(this);
    MoveComponent* mC = new MoveComponent(this);
    sc=sC;
    mc=mC;
    sc->SetTexture(mGame->GetTexture("Assets/Ship.png"));
}

void Ship::OnProcessInput(const Uint8 *keyState){
    float fSpeed=0;
    float rSpeed=0;
    if(keyState[SDL_SCANCODE_UP]){
        fSpeed+=SPEED;
        sc->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
        
    }
    if(keyState[SDL_SCANCODE_DOWN]){
        fSpeed-=SPEED;
        sc->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
    }
    if(mc->GetForwardSpeed()==0){
        sc->SetTexture(mGame->GetTexture("Assets/Ship.png"));
    }
    if(keyState[SDL_SCANCODE_LEFT]){
        rSpeed+=(float) (M_PI);
    }
    if(keyState[SDL_SCANCODE_RIGHT]){
       rSpeed-=(float) (M_PI);
    }
    //make sure the laser comes out one per second
    if(keyState[SDL_SCANCODE_SPACE]){
        if(enoughTime==true){
            Laser* laser = new Laser(mGame);
            laser->SetPosition(this->GetPosition());
            laser->SetRotation(this->GetRotation());
            timeElapsed=0.0;
            enoughTime=false;
        }
    }
    mc->SetForwardSpeed(fSpeed);
    mc->SetAngularSpeed(rSpeed);
}

void Ship::OnUpdate(float deltaTime){
    //check if the time has passed over one second
    timeElapsed+=deltaTime;
    if(timeElapsed>TIME_BREAK){
        enoughTime=true;
    }
}
