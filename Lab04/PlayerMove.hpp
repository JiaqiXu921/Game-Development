//
//  PlayerMove.hpp
//  Lab04
//
//  Created by Jiaqi Xu on 2/12/21.
//
#pragma once
#ifndef PlayerMove_hpp
#define PlayerMove_hpp

#include <stdio.h>
#include "MoveComponent.h"
#include "Actor.h"
#include "Player.hpp"
#include "Game.h"
#include "Block.hpp"
#include "Goomba.hpp"
#endif /* PlayerMove_hpp */
class PlayerMove : public MoveComponent{
public:
    PlayerMove(class Actor* owner);
    void Update(float deltaTime);
    void ProcessInput(const Uint8* keyState);
    void animation();
private:
    class Actor* mOwner;
    class Player* player;
    float fSpeed;
    float mYSpeed=0.0f;
    bool mSpacePressed=false;
    bool mInAir=false;
    bool left;
    bool right;
    bool dead;
};
