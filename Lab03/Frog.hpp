//
//  Frog.hpp
//  Lab03
//
//  Created by Jiaqi Xu on 2/5/21.
//
#pragma once
#ifndef Frog_hpp
#define Frog_hpp

#include <stdio.h>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "DeadFrog.hpp"
#endif /* Frog_hpp */
class Frog : public Actor {
public:
    Frog(class Game* game);
    void OnProcessInput(const Uint8 *keyState);
    void OnUpdate(float deltaTime);
private:
    class SpriteComponent* sc;
    class CollisionComponent* cc;
    bool lastFrameUp;
    bool lastFrameDown;
    bool lastFrameLeft;
    bool lastFrameRight;
    int FROG_WH=50;
    int FROG_INI_POS_X=448;
    int FROG_INI_POS_Y=928;
    int WATER_LIMIT_TOP=180;
    int WATER_LIMIT_BOTTOM=510;
    int OFFSET_LR=32;
    int ONE_HOP=64;
    int HOP_LIMIT_TOP=32;
    int HOP_LIMIT_BOTTOM=992;
    int HOP_LIMIT_LEFT=32;
    int HOP_LIMIT_RIGHT=864;
};
