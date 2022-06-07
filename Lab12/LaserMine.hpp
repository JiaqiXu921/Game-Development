//
//  LaserMine.hpp
//  Lab10
//
//  Created by Jiaqi Xu on 4/5/21.
//

#ifndef LaserMine_hpp
#define LaserMine_hpp

#include <stdio.h>
#include "Actor.h"
#endif /* LaserMine_hpp */
class LaserMine : public Actor {
public:
    LaserMine(class Game* game, Actor* parent);

private:
    class Game* mGame;
    class MeshComponent* mc;
    class LaserComponent* lc;
};
