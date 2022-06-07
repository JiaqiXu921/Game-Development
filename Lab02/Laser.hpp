//
//  Laser.hpp
//  Lab02
//
//  Created by Jiaqi Xu on 2/3/21.
//

#ifndef Laser_hpp
#define Laser_hpp

#include <stdio.h>
#include "Actor.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Asteroid.hpp"
#endif /* Laser_hpp */
class Laser : public Actor {
public:
    Laser(class Game* game);
    void OnUpdate(float deltaTime);
private:
    class SpriteComponent* scL;
    class MoveComponent* mcL;
    float TIME_ALIVE=0.0;
    float COLLIDE_DIS = 70;
    float FORWARD_SPEED =400.0f;
    float TIME_BREAK=1;
};
