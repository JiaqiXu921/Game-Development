#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{

}

void MoveComponent::Update(float deltaTime)
{
    //update the actor’s rotation
    float currentRotationSpeed=mOwner->GetRotation();
    currentRotationSpeed += mAngularSpeed * deltaTime;
    mOwner->SetRotation(currentRotationSpeed);

    //update the actor’s position
    Vector2 currentPos=mOwner->GetPosition();
    currentPos+=mOwner->GetForward()*mForwardSpeed*deltaTime;
    mOwner->SetPosition(currentPos);
}

