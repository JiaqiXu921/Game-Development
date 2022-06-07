//
//  PlayerMove.cpp
//  Lab07
//
//  Created by Jiaqi Xu on 3/15/21.
//
#include "PlayerMove.hpp"
#include "Math.h"
#include "Actor.h"
#include "Player.hpp"
#include <SDL2/SDL.h>
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include <string>
#include "Random.h"
#include "CameraComponent.hpp"
#include "Block.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "CheckPoint.hpp"
#include "HUD.hpp"
PlayerMove::PlayerMove(Actor* owner)
:MoveComponent(owner)
,mOwner(owner)
{
    ChangeState(MoveState::Falling);
    Mix_Chunk* run=mOwner->GetGame()->GetSound("Assets/Sounds/Running.wav");
    mRunningSFX = Mix_PlayChannel(-1, run, -1);
    Mix_Pause(mRunningSFX);
}

PlayerMove::~PlayerMove(){
    Mix_HaltChannel(mRunningSFX);
}
void PlayerMove::Update(float deltaTime){
    if(mCurrentState == MoveState::Falling){
        UpdateFalling(deltaTime);
    }
    if(mCurrentState == MoveState::OnGround){
        UpdateOnGround(deltaTime);
    }
    if(mCurrentState == MoveState::Jump){
        UpdateJump(deltaTime);
    }
    if(mCurrentState == MoveState::WallClimb){
        UpdateWallClimb(deltaTime);
    }
    if(mCurrentState == MoveState::WallRun){
        UpdateWallRun(deltaTime);
    }
    
    float zPos = mOwner->GetPosition().z;
    if(zPos < FALLING_LIMIT){
        mOwner->SetPosition(mOwner->GetGame()->getPlayer()->GetRespawnPos());
        mOwner->SetRotation(0.0f);
        mVelocity = Vector3::Zero;
        mPendingForces = Vector3::Zero;
        ChangeState(MoveState::Falling);
    }
        if(mOwner->GetGame()->GetCheckPoints().size()>1){
            nextCheckPoint = mOwner->GetGame()->GetCheckPoints().front();
            nextCheckPoint->setIsActive(true);
            Player* player = mOwner->GetGame()->getPlayer();
            CollisionComponent* playerCC=player->getCollision();
            CollisionComponent* nextCheckPointCC=nextCheckPoint->getCC();
            bool intersect = playerCC->Intersect(nextCheckPointCC);
            if(intersect){
                mOwner->GetGame()->getPlayer()->getHUD()->updateText();
                mOwner->GetGame()->textUpdated = true;
                textTimer = 5.0f;
                Mix_Chunk* hit=mOwner->GetGame()->GetSound("Assets/Sounds/CheckPoint.wav");
                Mix_PlayChannel(-1, hit, 0);
                player->SetRespawnPos(nextCheckPoint->GetPosition());
                nextCheckPoint->SetState(ActorState::Destroy);
                mOwner->GetGame()->checkPoints.pop();
                nextCheckPoint =mOwner->GetGame()->GetCheckPoints().front();
                nextCheckPoint->setIsActive(true);
                
            }
        }
        else if(mOwner->GetGame()->GetCheckPoints().size()==1){
            nextCheckPoint=mOwner->GetGame()->checkPoints.front();
            nextCheckPoint->setIsActive(true);
            Player* player = mOwner->GetGame()->getPlayer();
            CollisionComponent* playerCC=player->getCollision();
            CollisionComponent* nextCheckPointCC=nextCheckPoint->getCC();
            bool intersect = playerCC->Intersect(nextCheckPointCC);
            if(intersect){
                mOwner->GetGame()->getPlayer()->getHUD()->updateText();
                mOwner->GetGame()->textUpdated = true;
                textTimer = 5.0f;
                Mix_Chunk* hit=mOwner->GetGame()->GetSound("Assets/Sounds/CheckPoint.wav");
                Mix_PlayChannel(-1, hit, 0);
                if(!nextCheckPoint->GetLevelString().empty()){
                    mOwner->GetGame()->setNextLevel(nextCheckPoint->GetLevelString());
                }
                player->SetRespawnPos(nextCheckPoint->GetPosition());
                nextCheckPoint->SetState(ActorState::Destroy);
                mOwner->GetGame()->checkPoints.pop();
                
            }
        }
    if((mCurrentState == MoveState::OnGround && mVelocity.Length() > SOUND_RUN_LIMIT) || mCurrentState == MoveState::WallClimb || mCurrentState == MoveState::WallRun){
        Mix_Resume(mRunningSFX);
    }
    else{
        Mix_Pause(mRunningSFX);
    }
    
    if(mOwner->GetGame()->textUpdated){
        textTimer -= deltaTime;
        if(textTimer < 0.0f){
            textTimer = 5.0f;
            mOwner->GetGame()->textUpdated = false;
            mOwner->GetGame()->draw = false;
        }
    }
    
    
}

void PlayerMove::ProcessInput(const Uint8* keyState){
    if(keyState[SDL_SCANCODE_W]){
        AddForce(mOwner->GetForward()*speed);
    }
    else if(keyState[SDL_SCANCODE_S]){
        AddForce(mOwner->GetForward()*(-speed));
    }
    if(keyState[SDL_SCANCODE_W] && keyState[SDL_SCANCODE_S]){
        this->SetForwardSpeed(0);
    }
    if(!keyState[SDL_SCANCODE_W] && !keyState[SDL_SCANCODE_S]){
        this->SetForwardSpeed(0);
    }

    if(keyState[SDL_SCANCODE_D]){
        AddForce(mOwner->GetRight()*speed);
    }
    else if(keyState[SDL_SCANCODE_A]){
        AddForce(mOwner->GetRight()*(-speed));
    }
    if(keyState[SDL_SCANCODE_D] && keyState[SDL_SCANCODE_A]){
        this->SetStrafeSpeed(0);
    }
    if(!keyState[SDL_SCANCODE_D] && !keyState[SDL_SCANCODE_A]){
        this->SetStrafeSpeed(0);
    }

    if(keyState[SDL_SCANCODE_SPACE] && spacePressed == false){
        if(mCurrentState == MoveState::OnGround){
            Mix_Chunk* jump=mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav");
            Mix_PlayChannel(-1, jump, 0);
            AddForce(mJumpForce);
            ChangeState(MoveState::Jump);
        }

    }
    spacePressed = keyState[SDL_SCANCODE_SPACE];

    int x,y;
    SDL_GetRelativeMouseState(&x, &y);
    float x1 = (x/maxMouseMovement) * Math::Pi*radPerSec;
    SetAngularSpeed(x1);
    float y1 = (y/maxMouseMovement) * Math::Pi*radPerSec;
    mOwner->GetGame()->getPlayer()->getCC()->SetPitchSpeed(y1);
}

void PlayerMove::UpdateOnGround(float deltaTime){
    PhysicsUpdate(deltaTime);
    bool falling = true;
    for(Actor* block: mOwner->GetGame()->getBlocks()){
        CollSide collside =FixCollision(mOwner->GetGame()->getPlayer()->getCollision(), static_cast<Block*>(block)->getCC());
        if(collside == CollSide::Top){
            falling = false;
        }
        if(collside == CollSide::SideMaxY || collside == CollSide::SideMaxX ||collside == CollSide::SideMinY ||collside == CollSide::SideMinX){
            bool canClimb = CanWallClimb(collside);
            if(canClimb == true){
                mWallClimbTimer=0.0f;
                ChangeState(MoveState::WallClimb);
                return;
            }
        }
    }

    if(falling){
        ChangeState(MoveState::Falling);
    }
}

void PlayerMove::UpdateJump(float deltaTime){
    AddForce(mGravity);
    PhysicsUpdate(deltaTime);
    for(Actor* block: mOwner->GetGame()->getBlocks()){
        CollSide collside =FixCollision(mOwner->GetGame()->getPlayer()->getCollision(), static_cast<Block*>(block)->getCC());
        if(collside == CollSide::Bottom){
            mVelocity.z = 0.0f;
        }
        if(collside == CollSide::SideMaxY || collside == CollSide::SideMaxX ||collside == CollSide::SideMinY ||collside == CollSide::SideMinX){
            bool canClimb = CanWallClimb(collside);
            if(canClimb == true){
                mWallClimbTimer=0.0f;
                ChangeState(MoveState::WallClimb);
                return;
            }
            else{
                bool canRun = CanWallRun(collside);
                if(canRun){
                    wallRunSide = collside;
                    ChangeState(MoveState::WallRun);
                    mWallRunTimer=0.0f;
                    return;
                }
            }
        }
    }
    if(mVelocity.z <= 0.0f){
        ChangeState(MoveState::Falling);
    }
}

void PlayerMove::UpdateFalling(float deltaTime){
    AddForce(mGravity);
    PhysicsUpdate(deltaTime);
    for(Actor* block: mOwner->GetGame()->getBlocks()){
        CollSide collside =FixCollision(mOwner->GetGame()->getPlayer()->getCollision(), static_cast<Block*>(block)->getCC());
        if(collside == CollSide::Top){
            mVelocity.z = 0.0f;
            Mix_Chunk* land=mOwner->GetGame()->GetSound("Assets/Sounds/Land.wav");
            Mix_PlayChannel(-1, land, 0);
            ChangeState(MoveState::OnGround);
        }
    }
}

void PlayerMove::UpdateWallClimb(float deltaTime){
    AddForce(mGravity);
    if(mWallClimbTimer < WALL_CLIMB_LIMIT){
        AddForce(wallClimbForce);
    }
    PhysicsUpdate(deltaTime);
    bool falling = true;
    for(Actor* block: mOwner->GetGame()->getBlocks()){
        CollSide collside =FixCollision(mOwner->GetGame()->getPlayer()->getCollision(), static_cast<Block*>(block)->getCC());
        if(collside == CollSide::SideMaxY || collside == CollSide::SideMaxX ||collside == CollSide::SideMinY ||collside == CollSide::SideMinX){
            falling = false;
        }
    }
    if(falling || mVelocity.z <0.0f){
        mVelocity.z=0;
        ChangeState(MoveState::Falling);
    }
    mWallClimbTimer+=deltaTime;
}

bool PlayerMove::CanWallClimb(CollSide collside){
    Vector3 normal = getNormal(collside);
    bool canWallClimb = true;
    Vector3 xyVelocity =Vector3(mVelocity.x,mVelocity.y,0);
    //if the player is facing the side
    if(Vector3::Dot(normal,mOwner->GetForward()) < 0.9f){
        canWallClimb = false;
    }
    //check the same for xyVelocity
    if(Vector3::Dot(xyVelocity, normal) < 0.9f){
        canWallClimb = false;
    }
    if(xyVelocity.Length() < CLIMB_SPEED_LIMIT){
        canWallClimb = false;
    }
    return canWallClimb;
}
CollSide PlayerMove::FixCollision(class CollisionComponent* self, class CollisionComponent* block){
    Vector3 offset;
    Vector3 position = mOwner->GetPosition();
    CollSide collside = self->GetMinOverlap(block, offset);
    if(collside != CollSide::None){
        position += offset;
        mOwner->SetPosition(position);
    }
    if(collside == CollSide::SideMinY || collside == CollSide::SideMaxY){
        if(collside == CollSide::SideMinY){
            AddForce(Vector3(0.0f, -700.0f, 0.0f));
        }
        else if(collside == CollSide::SideMaxY){
            AddForce(Vector3(0.0f, 700.0f, 0.0f));
        }
    }
    if(collside == CollSide::SideMinX || collside == CollSide::SideMaxX){
        if(collside == CollSide::SideMinX){
            AddForce(Vector3(-700.0f, 0.0f, 0.0f));
        }
        else if (collside == CollSide::SideMaxX){
            AddForce(Vector3(700.0f, 0.0f, 0.0f));
        }
    }
    return collside;
}

void PlayerMove::AddForce(const Vector3& force){
    mPendingForces += force;
}

void PlayerMove::PhysicsUpdate(float deltaTime){
    mAcceleration = mPendingForces * (1.0f/mMass);
    mVelocity += mAcceleration * deltaTime;
    FixXYVelocity();
    Vector3 position = mOwner->GetPosition();
    position += mVelocity * deltaTime;
    mOwner->SetPosition(position);
    float currentRotationSpeed=mOwner->GetRotation();
    currentRotationSpeed += GetAngularSpeed() * deltaTime;
    mOwner->SetRotation(currentRotationSpeed);
    mPendingForces = Vector3::Zero;
}

void PlayerMove::FixXYVelocity(){
    Vector2 tempXYVelocity = Vector2(mVelocity.x, mVelocity.y);
    if(tempXYVelocity.Length()>maxVelocity){
        tempXYVelocity.Normalize();
        tempXYVelocity *= maxVelocity;
    }
    if(mCurrentState == MoveState::OnGround || mCurrentState == MoveState::WallClimb){
        if(Math::NearZero(mAcceleration.x) || ((mAcceleration.x<0 && tempXYVelocity.x>0)|| (mAcceleration.x>0 && tempXYVelocity.x<0))){
            tempXYVelocity.x *= brakingFactor;
        }
        if(Math::NearZero(mAcceleration.y)|| ((mAcceleration.y<0 &&tempXYVelocity.y>0)|| (mAcceleration.y>0 &&tempXYVelocity.y<0))){
            tempXYVelocity.y *= brakingFactor;
        }
    }
    mVelocity.x = tempXYVelocity.x;
    mVelocity.y = tempXYVelocity.y;
}

void PlayerMove::UpdateWallRun(float deltaTime){
    AddForce(mGravity);
    if(mWallRunTimer < 0.4f){
        AddForce(mWallRunForce);
    }
    PhysicsUpdate(deltaTime);
    for(Actor* block: mOwner->GetGame()->getBlocks()){
         FixCollision(mOwner->GetGame()->getPlayer()->getCollision(), static_cast<Block*>(block)->getCC());
    }
    if(mVelocity.z < 0.0f){
        ChangeState(MoveState::Falling);
    }
    mWallRunTimer+=deltaTime;
}

bool PlayerMove::CanWallRun(CollSide collside){
    Vector3 normal = getNormal(collside);
    bool canWallRun = true;
    Vector3 xyVelocity =Vector3(mVelocity.x,mVelocity.y,0);
    if(Math::Abs(Vector3::Dot(mOwner->GetForward(), normal)) > 0.9f){
        canWallRun = false;
    }
    if(Vector3::Dot(xyVelocity, normal) < 0.9f){
        canWallRun = false;
    }
    if(xyVelocity.Length()<CLIMB_SPEED_LIMIT){
        canWallRun = false;
    }
    return canWallRun;
}

Vector3 PlayerMove::getNormal(CollSide collside){
    Vector3 normal = Vector3::Zero;
    if(collside==CollSide::SideMaxX){
        normal=Vector3(-1,0,0);
    }
    if(collside==CollSide::SideMinX){
        normal=Vector3(1,0,0);
    }
    if(collside==CollSide::SideMaxY){
        normal=Vector3(0,-1,0);
    }
    if(collside==CollSide::SideMinY){
        normal=Vector3(0,1,0);
    }
    return normal;
}
