//
//  Asteroid.cpp
//  Lab02
//
//  Created by Jiaqi Xu on 2/3/21.
//

#include "Asteroid.hpp"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Random.h"
Asteroid::Asteroid(class Game* game)
:Actor(game)
{
    SpriteComponent* sc = new SpriteComponent(this);
    MoveComponent* mc = new MoveComponent(this);
    scA=sc;
    mcA=mc;
    //set random rotation
    mcA->SetForwardSpeed(FORWARD_SPEED);
    scA->SetTexture(mGame->GetTexture("Assets/Asteroid.png"));
    this->SetRotation(Random::GetFloatRange(0.0f,Math::TwoPi));
    Vector2 min(0,0);
    Vector2 max(WINDOW_WIDTH,WINDOW_HEIGHT);
    this->SetPosition(Random::GetVector(min, max));
    mGame->AddAsteroid(this);
}
//destructor
Asteroid::~Asteroid(){
    mGame->RemoveAsteroid(this);
    
}
void Asteroid::OnUpdate(float deltaTime){
    //if the asteroid goes off the screen, move it to another corner
    if(this->GetPosition().x<0){
        Vector2 newP(WINDOW_WIDTH-2,this->GetPosition().y);
        this->SetPosition(newP);
    }
    if(this->GetPosition().x>WINDOW_WIDTH-2){
        Vector2 newP(2,this->GetPosition().y);
        this->SetPosition(newP);
    }
    if(this->GetPosition().y<0){
        Vector2 newP(this->GetPosition().x,WINDOW_HEIGHT-2);
        this->SetPosition(newP);
    }
    if(this->GetPosition().y>WINDOW_HEIGHT-2){
        Vector2 newP(this->GetPosition().x,2);
        this->SetPosition(newP);
    }
}
