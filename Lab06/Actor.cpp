#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
Actor::Actor(Game* game)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector2::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
{
    mGame->AddActor(this);
}

//destructor
Actor::~Actor()
{
    //remove an actors' components
    mGame->RemoveActor(this);
    for(Component* comp:mComponents){
        delete comp;
    }
    //call clear
    mComponents.clear();
}

void Actor::Update(float deltaTime)
{
    //if the actor's state is active, update all the components
    if(mState==ActorState::Active){
        for(Component* comp:mComponents){
            comp->Update(deltaTime);
        }
        OnUpdate(deltaTime);
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

Vector2 Actor::GetForward(){
    return Vector2 (Math::Cos(mRotation),-Math::Sin(mRotation));
}
