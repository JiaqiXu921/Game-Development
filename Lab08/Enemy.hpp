//
//  Enemy.hpp
//  Lab08
//
//  Created by Jiaqi Xu on 3/23/21.
//

#ifndef Enemy_hpp
#define Enemy_hpp

#include <stdio.h>
#include "Actor.h"
#endif /* Enemy_hpp */
class Enemy : public Actor {
public:
    Enemy(class Game* game);
    class EnemyMove* getEM() {return em;}
private:
    class EnemyMove* em;
    class MeshComponent* mc;
};
