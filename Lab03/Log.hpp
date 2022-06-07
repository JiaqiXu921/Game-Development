//
//  Log.hpp
//  Lab03
//
//  Created by Jiaqi Xu on 2/5/21.
//
#pragma once
#ifndef Log_hpp
#define Log_hpp

#include <stdio.h>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "WrappingMove.hpp"
#include "CollisionComponent.h"
#endif /* Log_hpp */
class Log : public Actor {
public:
    Log(class Game* game);
    ~Log();
    CollisionComponent* getCC() {return cc;}
    SpriteComponent* getSC() {return sc;}
    WrappingMove* getWM() {return wm;}
private:
    float FORWARD_SPEED=75.0f;
    class CollisionComponent* cc;
    class SpriteComponent* sc;
    class WrappingMove* wm;
};
