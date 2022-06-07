//
//  Spawner.cpp
//  Lab04
//
//  Created by Jiaqi Xu on 2/17/21.
//

#include "Spawner.hpp"
Spawner::Spawner(class Game* game)
:Actor(game)
{
    mGame=game;
}

void Spawner::OnUpdate(float deltaTime){
    //generate a sqawner if mario gets close to the spawner
    if(abs(this->GetPosition().x-mGame->myPlayer->GetPosition().x)<600){
        Goomba* goomba=new Goomba(mGame);
        goomba->SetPosition(this->GetPosition());
        this->SetState(ActorState::Destroy);
    }
}
