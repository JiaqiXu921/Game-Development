//
//  GoombaMove.hpp
//  Lab04
//
//  Created by Jiaqi Xu on 2/17/21.
//
#pragma once
#ifndef GoombaMove_hpp
#define GoombaMove_hpp

#include <stdio.h>
#include "MoveComponent.h"
#include "Goomba.hpp"
#endif /* GoombaMove_hpp */
class GoombaMove : public MoveComponent
{
public:
    GoombaMove(class Actor* owner);
    void Update(float deltaTime);
  
private:
    class Actor* mOwner;
    class Goomba* goomba;
    float goombaSpeed=-100.0f;
    float mYSpeed=0.0f;
    float timePassed=0;
};
