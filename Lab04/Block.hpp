//
//  Block.hpp
//  Lab04
//
//  Created by Jiaqi Xu on 2/12/21.
//
#pragma once
#ifndef Block_hpp
#define Block_hpp

#include <stdio.h>
#include "Actor.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#endif /* Block_hpp */
class Block : public Actor {
public:
    Block(class Game* game);
    ~Block();
    CollisionComponent* getCC() {return cc;}
    void changeTex(std::string& filename);
private:
    class SpriteComponent* sc;
    class CollisionComponent* cc;
    
};
