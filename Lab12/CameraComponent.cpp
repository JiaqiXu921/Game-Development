//
//  CameraComponent.cpp
//  Lab08
//
//  Created by Jiaqi Xu on 3/19/21.
//

#include "CameraComponent.hpp"
#include "Component.h"
#include "Math.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.hpp"
#include "PlayerMove.hpp"
#include <iostream>

CameraComponent::CameraComponent(class Actor* owner)
:Component(owner)
{
    
}

void CameraComponent::Update(float deltaTime)
{
    CollSide wallRunSide=CollSide::None;
    mPitchAngle += mPitchSpeed * deltaTime;
    mPitchAngle=Math::Clamp(mPitchAngle, -Math::Pi/4, Math::Pi/4);
    Matrix4 pitch = Matrix4::CreateRotationY(mPitchAngle);
    Matrix4 yaw = Matrix4::CreateRotationZ(mOwner->GetRotation());
    Matrix4 combined = pitch*yaw;
    Vector3 forward = Vector3::Transform(Vector3::UnitX, combined);
    //camera change during wall run
    Vector3 sideRotation = Vector3::UnitZ;
    wallRunSide = mOwner->GetGame()->getPlayer()->getPM()->getWallRunSide();
    if(mOwner->GetGame()->getPlayer()->getPM()->getCurrentState() == mOwner->GetGame()->getPlayer()->getPM()->WallRun){
        if(wallRunSide == CollSide::SideMaxY || wallRunSide == CollSide::SideMinX){
            mWallRunAngle -= deltaTime;
         }
        else if(wallRunSide == CollSide::SideMinY ||wallRunSide == CollSide::SideMaxX){
            mWallRunAngle += deltaTime;
        }
        mWallRunAngle = Math::Clamp(mWallRunAngle, -Math::Pi/4, Math::Pi/4);
    }
    //reverse back the camera angle
    else{
        if(mWallRunAngle>0.005f || mWallRunAngle<-0.005f){
            if(wallRunSide == CollSide::SideMaxY || wallRunSide == CollSide::SideMinX){
                mWallRunAngle += deltaTime;
             }
            else if(wallRunSide == CollSide::SideMinY ||wallRunSide == CollSide::SideMaxX){
                mWallRunAngle -= deltaTime;
            }
        }
        else{
            mWallRunAngle=0;
        }
        
    }
    //change
    if(mOwner->GetGame()->getPlayer()->getPM()->getCurrentState() != mOwner->GetGame()->getPlayer()->getPM()->OnGround){
        if(wallRunSide == CollSide::SideMinX || wallRunSide == CollSide::SideMaxX){
            Matrix4 xzParallel =Matrix4::CreateRotationY(mWallRunAngle);
            sideRotation = Vector3::Transform(Vector3::UnitZ, xzParallel);
        }
        else if(wallRunSide == CollSide::SideMinY || wallRunSide == CollSide::SideMaxY){
            Matrix4 yzParallel =Matrix4::CreateRotationX(mWallRunAngle);
            sideRotation = Vector3::Transform(Vector3::UnitZ, yzParallel);
        }
    }
    
    cameraPos = mOwner->GetPosition();
    Vector3 targetPos =mOwner->GetPosition() + forward;
    Matrix4 viewMatrix = Matrix4::CreateLookAt(cameraPos, targetPos, sideRotation);
    mOwner->GetGame()->GetRenderer()->SetViewMatrix(viewMatrix);
}
