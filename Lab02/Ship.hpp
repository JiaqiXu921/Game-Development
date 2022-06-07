//
//  Ship.hpp
//  Lab02
//
//  Created by Jiaqi Xu on 2/2/21.
//

#ifndef Ship_hpp
#define Ship_hpp

#include <stdio.h>

#endif /* Ship_hpp */
#include "Actor.h"

class Ship : public Actor {
public:
    Ship(class Game* myGame);
    void OnProcessInput(const Uint8 *keyState);
    void OnUpdate(float deltaTime);
private:
    class SpriteComponent* sc;
    class MoveComponent* mc;
    float timeElapsed=0.0;
    bool enoughTime;
    float TIME_BREAK=1;
    float SPEED=400.f;
};
