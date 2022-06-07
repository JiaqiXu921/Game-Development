#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
,mDepth(0.0f)
{

}

CollisionComponent::~CollisionComponent()
{

}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	Vector3 aMin = GetMin();
	Vector3 aMax = GetMax();
	Vector3 bMin = other->GetMin();
	Vector3 bMax = other->GetMax();

	bool no = aMax.x < bMin.x ||
		aMax.y < bMin.y ||
		aMax.z < bMin.z ||
		bMax.x < aMin.x ||
		bMax.y < aMin.y ||
		bMax.z < aMin.z;

	return !no;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale() / 2.0f;
	v.y -= mWidth * mOwner->GetScale() / 2.0f;
	v.z -= mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale() / 2.0f;
	v.y += mWidth * mOwner->GetScale() / 2.0f;
	v.z += mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

const Vector3& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector3& offset)
{
	offset = Vector3::Zero;
    float otherMinXDiff=abs(other->GetMin().x-this->GetMax().x);
    float otherMinYDiff=abs(other->GetMin().y-this->GetMax().y);
    float otherMaxXDiff=abs(other->GetMax().x-this->GetMin().x);
    float otherMaxYDiff=abs(other->GetMax().y-this->GetMin().y);
    float otherMinZDiff=abs(other->GetMin().z-this->GetMax().z);
    float otherMaxZDiff=abs(other->GetMax().z-this->GetMin().z);

    std::vector<float> others={otherMinXDiff,otherMinYDiff,otherMinZDiff,otherMaxXDiff,otherMaxYDiff,otherMaxZDiff};
    auto it = std::min_element(others.begin(), others.end());
    float min = *it;

    if(!Intersect(other)){
        return CollSide::None;
    }
    if(min == otherMinXDiff){
        offset.x = other->GetMin().x-this->GetMax().x;
        return CollSide::SideMinX;
    }
    if(min == otherMinYDiff){
        offset.y = other->GetMin().y-this->GetMax().y;
        return CollSide::SideMinY;
    }
    if(min == otherMaxXDiff){
        offset.x =other->GetMax().x-this->GetMin().x;
        return CollSide::SideMaxX;
    }
    if(min == otherMaxYDiff){
        offset.y =other->GetMax().y-this->GetMin().y;
        return CollSide::SideMaxY;
    }
    if(min == otherMaxZDiff){
        offset.z =other->GetMax().z-this->GetMin().z;
        return CollSide::Top;
    }
    if(min == otherMinZDiff){
        offset.z =other->GetMin().z-this->GetMax().z;
        return CollSide::Bottom;
    }
	return CollSide::None;

}
