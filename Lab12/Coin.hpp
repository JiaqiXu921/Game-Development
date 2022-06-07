//
//  Coin.hpp
//  Lab11
//
//  Created by Jiaqi Xu on 4/12/21.
//

#ifndef Coin_hpp
#define Coin_hpp

#include <stdio.h>
#include "Actor.h"
#endif /* Coin_hpp */
class Coin : public Actor {
public:
    Coin(class Game* game, Actor* parent);
    virtual void OnUpdate(float deltaTime) override;

private:
    class MeshComponent* mc;
    class CollisionComponent* cc;
};
