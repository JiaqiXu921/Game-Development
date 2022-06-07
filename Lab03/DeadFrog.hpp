//
//  DeadFrog.hpp
//  Lab03
//
//  Created by Jiaqi Xu on 2/9/21.
//
#pragma once
#ifndef DeadFrog_hpp
#define DeadFrog_hpp

#include <stdio.h>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Vehicle.hpp"
#endif /* DeadFrog_hpp */
class DeadFrog : public Actor {
public:
    DeadFrog(class Game* game);
    void OnUpdate(float deltaTime);
private:
    class SpriteComponent* scDead;
    float timeAlive=0.0;
};
