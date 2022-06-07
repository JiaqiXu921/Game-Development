//
//  Spawner.hpp
//  Lab04
//
//  Created by Jiaqi Xu on 2/17/21.
//

#ifndef Spawner_hpp
#define Spawner_hpp

#include <stdio.h>
#include "Actor.h"
#include "Game.h"
#include "Goomba.hpp"
#endif /* Spawner_hpp */
class Spawner : public Actor {
public:
    Spawner(class Game* game);
    void OnUpdate(float deltaTime) override;
private:
    class Game* mGame;
};
