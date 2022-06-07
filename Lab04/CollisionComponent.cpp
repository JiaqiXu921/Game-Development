#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
    if(this->GetMax().x<other->GetMin().x){
        return false;
    }
    else if(other->GetMax().x < this->GetMin().x){
        return false;
    }
    else if(this->GetMax().y < other->GetMin().y){
        return false;
    }
    else if(other->GetMax().y < this->GetMin().y){
        return false;
    }
    else{
        return true;
    }
}

Vector2 CollisionComponent::GetMin() const
{
    float minX = mOwner->GetPosition().x - (mWidth * mOwner->GetScale()) / 2.0f;
    float minY = mOwner->GetPosition().y - (mHeight * mOwner->GetScale()) / 2.0f;
	return Vector2(minX,minY);
}

Vector2 CollisionComponent::GetMax() const
{
    float maxX=mOwner->GetPosition().x + (mWidth * mOwner->GetScale()) / 2.0f;
    float maxY=mOwner->GetPosition().y + (mHeight * mOwner->GetScale()) / 2.0f;
	return Vector2(maxX,maxY);
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector2& offset)
{
	offset = Vector2::Zero;
    CollSide collside;
    float otherMaxXDiff=other->GetMax().x-this->GetMin().x;
    float otherMinXDiff=other->GetMin().x-this->GetMax().x;
    float otherMaxYDiff=other->GetMax().y-this->GetMin().y;
    float otherMinYDiff=other->GetMin().y-this->GetMax().y;
    
    float min=Math::Min<float>(abs(otherMaxXDiff), Math::Min(abs(otherMinXDiff), Math::Min(abs(otherMaxYDiff), abs(otherMinYDiff))));
    
    if(!this->Intersect(other)){
        collside=CollSide::None;
    }
    if(min==abs(otherMaxXDiff)){
        offset.x=otherMaxXDiff;
        collside=CollSide::Right;
    }
    if(min==abs(otherMinXDiff)){
        offset.x=otherMinXDiff;
        collside=CollSide::Left;
    }
    if(min==abs(otherMaxYDiff)){
        offset.y=otherMaxYDiff;
        collside=CollSide::Bottom;
    }
    if(min==abs(otherMinYDiff)){
        offset.y=otherMinYDiff;
        collside=CollSide::Top;
    }
	return collside;
}
