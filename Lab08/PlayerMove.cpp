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
#include "HeightMap.hpp"
#include "PlayerUI.h"
#include "EnemyMove.hpp"
#include "Enemy.hpp"
PlayerMove::PlayerMove(Actor* owner)
:VehicleMove(owner)
,mOwner(owner)
{
//    player = (Player*)mOwner;
//    velocity = Vector3(defaultSpeed,0,0);
    mOwner->SetPosition(mOwner->GetGame()->getHeightMap()->CellToWorld(39, 58));
}


void PlayerMove::Update(float deltaTime){
    if(Final){
        Final = false;
        int oldChannel =mOwner->GetGame()->playing;
        Mix_FadeOutChannel(mOwner->GetGame()->playing, 250);
        Mix_PlayChannel(-1, mOwner->GetGame()->getFinalLap(), 0);
        mOwner->GetGame()->playing = Mix_FadeInChannel(oldChannel, mOwner->GetGame()->getMusicFast(), -1, 4000);
    }
    VehicleMove::Update(deltaTime);
}

void PlayerMove::ProcessInput(const Uint8* keyState){
    if(keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]){
        setPedal(true);
    }
    else{
        setPedal(false);
    }
    
    if(keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]){
        setDirection(Direction::Left);
    }
    else if(keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]){
        setDirection(Direction::Right);
    }
    else{
        setDirection(Direction::None);
    }
    if((keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_D]) || (keyState[SDL_SCANCODE_LEFT] && keyState[SDL_SCANCODE_RIGHT])){
       setDirection(Direction::None);
    }
}

void PlayerMove::OnLapChange(int newLap){
    if(newLap < 5){
        mOwner->GetGame()->getPlayer()->getUI()->OnLapChange(VehicleMove::getCurrLap());
    }
    else if (newLap >= 5 && newLap > mOwner->GetGame()->getEnemy()->getEM()->getCurrLap()) {
        mOwner->GetGame()->getPlayer()->getUI()->SetRaceState(PlayerUI::Won);
        Mix_FadeOutChannel(mOwner->GetGame()->playing, 250);
        Mix_PlayChannel(-1, mOwner->GetGame()->getWon(), 0);
        mOwner->GetGame()->getPlayer()->SetState(ActorState::Paused);
        mOwner->GetGame()->getEnemy()->SetState(ActorState::Paused);
    }
    else if (newLap >= 5 && mOwner->GetGame()->getEnemy()->getEM()->getCurrLap() >= 5){
        mOwner->GetGame()->getPlayer()->getUI()->SetRaceState(PlayerUI::Lost);
        Mix_FadeOutChannel(mOwner->GetGame()->playing, 250);
        Mix_PlayChannel(-1, mOwner->GetGame()->getLost(), 0);
        mOwner->GetGame()->getPlayer()->SetState(ActorState::Paused);
        mOwner->GetGame()->getEnemy()->SetState(ActorState::Paused);
    }
}
