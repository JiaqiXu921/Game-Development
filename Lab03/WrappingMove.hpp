//
//  WrappingMove.hpp
//  Lab03
//
//  Created by Jiaqi Xu on 2/9/21.
//
#pragma once
#ifndef WrappingMove_hpp
#define WrappingMove_hpp

#include <stdio.h>
#include "MoveComponent.h"
#include "Actor.h"
#endif /* WrappingMove_hpp */
class WrappingMove : public MoveComponent {
public:
    WrappingMove(class Actor* owner);
    void Update(float deltaTime);
    Vector2 direction;
private:
    int WINDOW_WIDTH=896;
};
