//
//  PlayerMove.cpp
//  Lab04
//
//  Created by Jiaqi Xu on 2/12/21.
//

#include "PlayerMove.hpp"

PlayerMove::PlayerMove(Actor* owner)
:MoveComponent(owner)
,mOwner(owner)
{
    player = (Player*)mOwner;
}


void PlayerMove::Update(float deltaTime){
    left=false;
    right=false;
    //update x and y position
    Vector2 position=mOwner->GetPosition();
    position.x+=fSpeed*deltaTime;
    position.y+=mYSpeed*deltaTime;
    //check if mario is moving to the left or right
    if(position.x<mOwner->GetPosition().x){
        left=true;
    }
    else if(position.x>mOwner->GetPosition().x){
        right=true;
    }
    mOwner->SetPosition(position);
    //check if mario falls off the screen
    if(mOwner->GetPosition().y>448.0f){
        mInAir=false;
        dead=true;
    }
    //check winning condition
    if(position.x>6368){
        Mix_HaltChannel(mOwner->GetGame()->getBackgroundSound());
        Mix_Chunk* winS=mOwner->GetGame()->GetSound("Assets/Sounds/StageClear.wav");
        Mix_PlayChannel(-1, winS, 0);
        player->SetState(ActorState::Paused);
    }
    //local bool
    bool collide=false;
    //GetMinOverlap
    CollSide collside;
    Vector2 offset=Vector2::Zero;
    //check collision
    for(Block* block: mOwner->GetGame()->getBlocks()){
        bool intersect=player->cc->Intersect(block->getCC());
               if(intersect==true){
                   collside=player->cc->GetMinOverlap(block->getCC(), offset);
                        if(collside==CollSide::None){
                            continue;
                        }
                        else if(collside==CollSide::Top && mYSpeed > 0.0f){
                            mInAir=false;
                            collide=true;
                            mYSpeed=0;
                        }
                        else if(collside==CollSide::Bottom){
                            Mix_Chunk* bumpS=mOwner->GetGame()->GetSound("Assets/Sounds/Bump.wav");
                            Mix_PlayChannel(-1,bumpS,0);
                            if(mYSpeed<0.0f){
                                mYSpeed=0.0f;
                            }
                            collide=true;
                        }
                        else if(collside==CollSide::Left||collside==CollSide::Right){
                            collide=true;
                        }
                   position+=offset;
                   mOwner->SetPosition(position);
               }
    }
    //check collision with goombas
    for(Goomba* goomba: mOwner->GetGame()->getGoombas()){
        bool intersect=player->cc->Intersect(goomba->getCC());
        if(intersect==true){
            collside=player->cc->GetMinOverlap(goomba->getCC(), offset);
            if(collside==CollSide::None){
                continue;
            }
            else if(collside==CollSide::Top || ((collside==CollSide::Left||collside==CollSide::Right) && mInAir==true)){
                Mix_Chunk* stompS=mOwner->GetGame()->GetSound("Assets/Sounds/Stomp.wav");
                Mix_PlayChannel(-1,stompS,0);
                goomba->setIsStomped(true);
                mYSpeed=-350.0f;
                mInAir=true;
                position += offset;
                mOwner->SetPosition(position);
            }
            else{
                dead=true;
                player->SetState(ActorState::Paused);
            }
        }
    }
    if(collide==false){
        mInAir=true;
    }
    //add gravity
    mYSpeed+=2000.0f*deltaTime;
    //scrolling camera
    mOwner->GetGame()->setCameraPositionX(Math::Max(mOwner->GetGame()->getCameraPosition().x, mOwner->GetPosition().x - 300));
    if(mOwner->GetPosition().x<mOwner->GetGame()->getCameraPosition().x){
        mOwner->SetPosition(Vector2(mOwner->GetGame()->getCameraPosition().x,mOwner->GetPosition().y));
    }
    animation();
    //set dead condition
    if(dead==true){
        Mix_HaltChannel(mOwner->GetGame()->getBackgroundSound());
        Mix_Chunk* deadS=mOwner->GetGame()->GetSound("Assets/Sounds/Dead.wav");
        Mix_PlayChannel(-1, deadS, 0);
        player->SetState(ActorState::Paused);
        
    }
}

void PlayerMove::ProcessInput(const Uint8* keyState){
    fSpeed=0.0f;
    if(keyState[SDL_SCANCODE_LEFT]){
        fSpeed-=300.0f;
    }
    else if(keyState[SDL_SCANCODE_RIGHT]){
        fSpeed+=300.0f;
    }
    else{
        fSpeed=0.0f;
    }
    this->SetForwardSpeed(fSpeed);
    
    if(keyState[SDL_SCANCODE_SPACE]){
        if(mSpacePressed==false){
            if(mInAir==false){
                Mix_Chunk* jumpS=mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav");
                Mix_PlayChannel(-1,jumpS,0);
                mYSpeed=-700.0f;
            }
        }
        mInAir=true;
    }
    mSpacePressed=keyState[SDL_SCANCODE_SPACE];
}
//add animation
void PlayerMove::animation(){
    if(mInAir==false){
        if(right==true){
            player->getAsc()->SetAnimation("runRight");
        }
        else if(left==true){
            player->getAsc()->SetAnimation("runLeft");
        }
        else{
            player->getAsc()->SetAnimation("idle");
        }
    }
    else if(mInAir==true){
        if(left==true){
            player->getAsc()->SetAnimation("jumpLeft");
        }
        else if(right==true){
            player->getAsc()->SetAnimation("jumpRight");
        }
        else{
            if(player->getAsc()->GetAnimName()=="runRight" || player->getAsc()->GetAnimName()=="jumpRight" || player->getAsc()->GetAnimName()=="idle"){
                player->getAsc()->SetAnimation("jumpRight");
            }
            else{
                player->getAsc()->SetAnimation("jumpLeft");
            }
        }
    }
    if(dead==true){
        player->getAsc()->SetAnimation("dead");
    }
}
