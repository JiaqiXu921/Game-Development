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
    float otherMaxXDiff=abs(other->GetMax().x-this->GetMin().x);
    float otherMinXDiff=abs(other->GetMin().x-this->GetMax().x);
    float otherMaxYDiff=abs(other->GetMax().y-this->GetMin().y);
    float otherMinYDiff=abs(other->GetMin().y-this->GetMax().y);
    
    float min=Math::Min<float>(otherMaxXDiff, Math::Min(otherMinXDiff, Math::Min(otherMaxYDiff, otherMinYDiff)));
    
    if(!this->Intersect(other)){
        collside=CollSide::None;
    }
    if(min==otherMaxXDiff){
        offset.x=other->GetMax().x-this->GetMin().x;
        collside=CollSide::Right;
    }
    if(min==otherMinXDiff){
        offset.x=other->GetMin().x-this->GetMax().x;
        collside=CollSide::Left;
    }
    if(min==otherMaxYDiff){
        offset.y=other->GetMax().y-this->GetMin().y;
        collside=CollSide::Bottom;
    }
    if(min==otherMinYDiff){
        offset.y=other->GetMin().y-this->GetMax().y;
        collside=CollSide::Top;
    }
	return collside;
}
