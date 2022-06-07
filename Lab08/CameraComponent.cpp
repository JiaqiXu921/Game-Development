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
CameraComponent::CameraComponent(class Actor* owner)
:Component(owner)
{
    
}

void CameraComponent::Update(float deltaTime)
{
    Vector3 ideal = mOwner->GetPosition()-mOwner->GetForward()*hDist+Vector3::UnitZ * 70.0f;
    Vector3 displacement=cameraPos - ideal;
    Vector3 springAcceleration = (-springConst*displacement)-(dampConst*cameraVelocity);
    cameraVelocity +=springAcceleration*deltaTime;
    cameraPos +=cameraVelocity*deltaTime;
    Vector3 targetPos = mOwner->GetPosition() + mOwner->GetForward() * offset;
    Matrix4 viewMatrix = Matrix4::CreateLookAt(cameraPos, targetPos, Vector3::UnitZ);
    mOwner->GetGame()->GetRenderer()->SetViewMatrix(viewMatrix);
}

void CameraComponent::SnapToIdeal(){
    Vector3 ideal = mOwner->GetPosition() - (mOwner->GetForward() * hDist) + (Vector3::UnitZ * 70.0f);
    cameraPos=ideal;
    Vector3 targetPos = mOwner->GetPosition() + mOwner->GetForward() * offset;
    Matrix4 viewMatrix = Matrix4::CreateLookAt(ideal, targetPos, Vector3::UnitZ);
    mOwner->GetGame()->GetRenderer()->SetViewMatrix(viewMatrix);
}
