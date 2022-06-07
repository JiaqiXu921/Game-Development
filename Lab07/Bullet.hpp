//
//  Bullet.hpp
//  Lab07
//
//  Created by Jiaqi Xu on 3/16/21.
//

#ifndef Bullet_hpp
#define Bullet_hpp

#include <stdio.h>
#include "Actor.h"
#endif /* Bullet_hpp */
class Bullet : public Actor {
public:
     Bullet(class Game* game);
    void OnUpdate(float deltaTime) override;

private:
    class Game* mGame;
    class MeshComponent* mc;
    class CollisionComponent* cc;
    class MoveComponent* move;
    float TIME_ALIVE=0;
    int defaultSpeed=400;
};
