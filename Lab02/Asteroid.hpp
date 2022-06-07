//
//  Asteroid.hpp
//  Lab02
//
//  Created by Jiaqi Xu on 2/3/21.
//

#ifndef Asteroid_hpp
#define Asteroid_hpp

#include <stdio.h>
#include "Actor.h"
#endif /* Asteroid_hpp */
class Asteroid : public Actor {
public:
    Asteroid(class Game* game);
    ~Asteroid();
    void OnUpdate(float deltaTime);
private:
    class SpriteComponent* scA;
    class MoveComponent* mcA;
    int WINDOW_WIDTH=1024;
    int WINDOW_HEIGHT=768;
    float FORWARD_SPEED=150.f;
};
