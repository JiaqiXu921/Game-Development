//
//  Vehicle.hpp
//  itp380labs
//
//  Created by Jiaqi Xu on 2/5/21.
//
#pragma once
#ifndef Vehicle_hpp
#define Vehicle_hpp

#include <stdio.h>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Frog.hpp"
#endif /* Vehicle_hpp */
class Vehicle : public Actor {
public:
    Vehicle(class Game* game);
    class CollisionComponent* cc;
    ~Vehicle();
    void OnUpdate(float deltaTime);
private:
    class SpriteComponent* sc;
    class WrappingMove* wm;
    float FORWARD_SPEED = 100.0f;
    int CAR_WH=64;
    int TRUCK_W=128;
    int TRUCK_H=48;
};
