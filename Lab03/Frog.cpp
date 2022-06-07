//
//  Frog.cpp
//  Lab03
//
//  Created by Jiaqi Xu on 2/5/21.
//

#include "Frog.hpp"
#include "SpriteComponent.h"
#include <fstream>
#include <iostream>
#include <sstream>
Frog::Frog(class Game* game)
:Actor(game)
{
    SpriteComponent* sC = new SpriteComponent(this);
    sc=sC;
    sc->SetTexture(mGame->GetTexture("Assets/Frog.png"));
    CollisionComponent* cC=new CollisionComponent(this);
    cc=cC;
    cc->SetSize(FROG_WH,FROG_WH);
}
void Frog::OnProcessInput(const Uint8 *keyState){
    if(keyState[SDL_SCANCODE_UP]){
        if(lastFrameUp==false){
            if(this->GetPosition().y>HOP_LIMIT_TOP){
                float newPosY=this->GetPosition().y-ONE_HOP;
                Vector2 pos(this->GetPosition().x,newPosY);
                this->SetPosition(pos);
            }
        }
    }
    if(keyState[SDL_SCANCODE_DOWN]){
        if(lastFrameDown==false){
            if(this->GetPosition().y<HOP_LIMIT_BOTTOM){
                float newPosY=this->GetPosition().y+ONE_HOP;
                Vector2 pos(this->GetPosition().x,newPosY);
                this->SetPosition(pos);
            }
        }
    }
    if(keyState[SDL_SCANCODE_LEFT]){
        if(lastFrameLeft==false){
            if(this->GetPosition().x>HOP_LIMIT_LEFT){
                float newPosX=this->GetPosition().x-ONE_HOP;
                Vector2 pos(newPosX,this->GetPosition().y);
                this->SetPosition(pos);
            }
        }
    }
    if(keyState[SDL_SCANCODE_RIGHT]){
        if(lastFrameRight==false){
            if(this->GetPosition().x<HOP_LIMIT_RIGHT){
                float newPosX=this->GetPosition().x+ONE_HOP;
                Vector2 pos(newPosX,this->GetPosition().y);
                this->SetPosition(pos);
            }
        }
    }
    //keep track of last frame for detecting leading edges
    lastFrameUp=keyState[SDL_SCANCODE_UP];
    lastFrameDown=keyState[SDL_SCANCODE_DOWN];
    lastFrameLeft=keyState[SDL_SCANCODE_LEFT];
    lastFrameRight=keyState[SDL_SCANCODE_RIGHT];
    
}

void Frog::OnUpdate(float deltaTime){
    //if the vehicle hits the frog, the frog dies
    for(Vehicle* entry: mGame->getVehicles()){
        bool intersect=cc->Intersect(entry->cc);
        if(intersect == true){
            DeadFrog* dead=new DeadFrog(mGame);
            dead->SetPosition(this->GetPosition());
            Vector2 original(FROG_INI_POS_X,FROG_INI_POS_Y);
            this->SetPosition(original);
        }
    }
    CollSide collside;
    Vector2 offset=Vector2::Zero;
    //loop over all the logs
    unsigned int counter=0;
    for(Log* log:mGame->getLogs()){
        //check if collision occured
        bool intersect=cc->Intersect(log->getCC());
        if(intersect==true){
            //check which side the frog collides with the log
            collside=cc->GetMinOverlap(log->getCC(), offset);
            if(collside==CollSide::None){
                continue;
            }
            else if(collside==CollSide::Left){
                offset.x+=OFFSET_LR;
            }
            else if(collside==CollSide::Right){
                offset.x-=OFFSET_LR;
            }
            else if(collside==CollSide::Left||collside==CollSide::Right||collside==CollSide::Top||collside==CollSide::Bottom){
                //set frog's y position to be the log's y position
                Vector2 newPos(this->GetPosition().x,log->GetPosition().y);
                //make the frog move with log in the same direction and speed
                newPos+=log->getWM()->direction*log->getWM()->GetForwardSpeed()*deltaTime;
                this->SetPosition(newPos);
            }
            
        }
        else{
            counter++;
        }
        //if the frog hits water, it dies
        if(counter==mGame->getLogs().size() && (this->GetPosition().y<WATER_LIMIT_BOTTOM && WATER_LIMIT_TOP<this->GetPosition().y)){
            DeadFrog* dead=new DeadFrog(mGame);
            dead->SetPosition(this->GetPosition());
            Vector2 original(FROG_INI_POS_X,FROG_INI_POS_Y);
            this->SetPosition(original);
        }
        //pause frog's position if it hits the goal
        bool intersectGoal=this->cc->Intersect(mGame->getGoal()->getCC());
        if(intersectGoal==true){
            this->SetPosition(mGame->getGoal()->GetPosition());
            this->SetState(ActorState::Paused);
        }
        //if the frog hits the wall, it dies
        else if(this->GetPosition().y<WATER_LIMIT_TOP){
            DeadFrog* dead=new DeadFrog(mGame);
            dead->SetPosition(this->GetPosition());
            Vector2 original(FROG_INI_POS_X,FROG_INI_POS_Y);
            this->SetPosition(original);
        }
        
    }
}
