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
#include "SideBlock.hpp"
#include "MeshComponent.h"
#include "Block.hpp"
#include <string>
#include "Random.h"
#include "Bullet.hpp"
PlayerMove::PlayerMove(Actor* owner)
:MoveComponent(owner)
,mOwner(owner)
{
    player = (Player*)mOwner;
    velocity = Vector3(defaultSpeed,0,0);
}


void PlayerMove::Update(float deltaTime){
    //increase speed
    timer+=deltaTime;
    if(timer>BULLET_TIMER){
        timeMultiplier+=0.15f;
        timer=0.0f;
    }
    
    //update position
    Vector3 position = mOwner->GetPosition();
    position += velocity*timeMultiplier*deltaTime;
    if(position.y < -Y_LIMIT){
        position.y = -Y_LIMIT;
    }
    if(position.y > Y_LIMIT){
        position.y = Y_LIMIT;
    }
    if(position.z < -Z_LIMIT){
        position.z = -Z_LIMIT;
    }
    if(position.z > Z_LIMIT){
        position.z = Z_LIMIT;
    }
    mOwner->SetPosition(position);
    
    //update camera position
    Vector3 cameraPos = mOwner->GetPosition()-Vector3::UnitX*300+Vector3::UnitZ*100;
    Vector3 targetPos = mOwner->GetPosition() + Vector3::UnitX * 20;
    Matrix4 viewMatrix = Matrix4::CreateLookAt(cameraPos, targetPos, Vector3::UnitZ);
    mOwner->GetGame()->GetRenderer()->SetViewMatrix(viewMatrix);
    
    //dynamically add sideblocks
    if(sideblockX - mOwner->GetPosition().x < generateBound){
        SideBlock* blockLeft = new SideBlock(mOwner->GetGame());
        SideBlock* blockRight = new SideBlock(mOwner->GetGame());
        blockLeft->SetPosition(Vector3(sideblockX,sideBlockSize,0));
        blockRight->SetPosition(Vector3(sideblockX,-sideBlockSize,0));
        if(colorIndex>totalColors){
            colorIndex=1;
        }
        blockLeft->getMC()->SetTextureIndex(colorIndex);
        blockRight->getMC()->SetTextureIndex(colorIndex);
        sideblockX += sideBlockSize;
        colorIndex++;
    }
    
    //dynamically add blocks
    if(blockX - mOwner->GetPosition().x < generateBound){
        std::string fileName = "Assets/Blocks/"+std::to_string(fileNum)+".txt";
        mOwner->GetGame()->increaseXGap();
        mOwner->GetGame()->LoadBlocks(fileName);
        fileNum++;
        
        if(fileNum>totalFiles){
            random = true;
        }
        if(random){
            fileNum=Random::GetIntRange(1, totalFiles);
        }
        
        blockX += blockSize;
    }
    
    //check for player's collision with blocks
    for(Block* block: mOwner->GetGame()->getBlocks()){
        bool intersect = mOwner->GetComponent<CollisionComponent>()->Intersect(block->getCC());
        if(intersect){
            mOwner->SetState(ActorState::Paused);
            Mix_HaltChannel(mOwner->GetGame()->getBackgroundSound());
            Mix_Chunk* dead=mOwner->GetGame()->GetSound("Assets/Sounds/ShipDie.wav");
            Mix_PlayChannel(-1, dead, 0);
        }
    }
}

void PlayerMove::ProcessInput(const Uint8* keyState){
    if(keyState[SDL_SCANCODE_W]){
        velocity.z = movementSpeed*timeMultiplier;
    }
    else if(keyState[SDL_SCANCODE_A]){
        velocity.y = -movementSpeed*timeMultiplier;
    }
    else if(keyState[SDL_SCANCODE_S]){
        velocity.z = -movementSpeed*timeMultiplier;
    }
    else if(keyState[SDL_SCANCODE_D]){
        velocity.y = movementSpeed*timeMultiplier;
    }
    else{
        velocity.y=0;
        velocity.z=0;
    }
    
    //shoot bullets
    if(keyState[SDL_SCANCODE_SPACE]){
        if(spacePressed==false){
            Bullet* bullet = new Bullet(mOwner->GetGame());
            bullet->SetPosition(mOwner->GetPosition());
            Mix_Chunk* shoot=mOwner->GetGame()->GetSound("Assets/Sounds/Shoot.wav");
            Mix_PlayChannel(-1, shoot, 0);
        }
    }
    spacePressed=keyState[SDL_SCANCODE_SPACE];
}
