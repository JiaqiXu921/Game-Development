#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
Actor::Actor(Game* game, Actor* parent)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector3::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
{
    mParent = parent;
    if(mParent==nullptr){
        mGame->AddActor(this);
    }
    else{
        mParent->AddChild(this);
    }
    
}

//destructor
Actor::~Actor()
{
    while(!mChildren.empty()){
        mChildren.pop_back();
    }
    //remove an actors' components
    if(mParent==nullptr){
        mGame->RemoveActor(this);
    }
    else{
        mParent->RemoveChild(this);
    }
    for(Component* comp:mComponents){
        delete comp;
    }
    //call clear
    mComponents.clear();
}

void Actor::Update(float deltaTime)
{
    CalcWorldTransform();
    //if the actor's state is active, update all the components
    if(mState==ActorState::Active){
        for(Component* comp:mComponents){
            comp->Update(deltaTime);
        }
        OnUpdate(deltaTime);
    }
    CalcWorldTransform();
    for(Actor* child:mChildren){
        child->Update(deltaTime);
    }
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
	if(mState==ActorState::Active){
        for(Component* comp:mComponents){
            comp->ProcessInput(keyState);
        }
        OnProcessInput(keyState);
    }
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}

Vector3 Actor::GetForward(){
    return Vector3 (Math::Cos(mRotation),Math::Sin(mRotation),0.0f);
}

Vector3 Actor::GetRight(){
    return Vector3(Math::Cos(Math::PiOver2+mRotation),Math::Sin(Math::PiOver2+mRotation),0.0f);}

Vector3 Actor::GetQuatForward(){
    Vector3 rotated = Vector3::Transform(Vector3::UnitX, quaternion);
    rotated.Normalize();
    return rotated;
}

void Actor::CalcWorldTransform(){
    Matrix4 scaleMatrix = Matrix4::CreateScale(mScale);
    Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
    Matrix4 quaternionM = Matrix4::CreateFromQuaternion(this->GetQuaternion());
    Matrix4 positionMatrix = Matrix4::CreateTranslation(mPosition);
    mWorldTransform= scaleMatrix * rotationMatrix*quaternionM * positionMatrix;
    if(mParent!=nullptr){
        if(mInheritScale){
            mWorldTransform *= mParent->mWorldTransform;
        }
        else{
            mWorldTransform *= mParent->GetWorldRotTrans();
        }
    }
}

Matrix4 Actor::GetWorldRotTrans(){
    Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
    Matrix4 quaternionM = Matrix4::CreateFromQuaternion(this->GetQuaternion());
    Matrix4 positionMatrix = Matrix4::CreateTranslation(mPosition);
    Matrix4 mParentWorldTransform= rotationMatrix * quaternionM * positionMatrix;
    if(mParent!=nullptr){
        mParentWorldTransform *=mParent->GetWorldRotTrans();
    }
    return mParentWorldTransform;
}

Vector3 Actor::GetWorldPosition(){
    return mWorldTransform.GetTranslation();
}

Vector3 Actor::GetWorldForward(){
    return mWorldTransform.GetXAxis();
}

void Actor::AddChild(Actor* child){
    mChildren.push_back(child);
}

void Actor::RemoveChild(Actor* child){
    std::vector<Actor*>::iterator sctoErase=std::find(mChildren.begin(), mChildren.end(), child);
    if(sctoErase!=mChildren.end()){
        mChildren.erase(sctoErase);
    }
}

