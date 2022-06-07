//
//  Block.hpp
//  Lab07
//
//  Created by Jiaqi Xu on 3/15/21.
//

#ifndef Block_hpp
#define Block_hpp

#include <stdio.h>
#include "Actor.h"
#endif /* Block_hpp */
class Block : public Actor {
public:
    Block(class Game* game);
    void OnUpdate(float deltaTime) override;
    class MeshComponent* getMC() {return mc;}
    void setExplode(bool input) {exploding = input;}
    class CollisionComponent* getCC() {return cc;}
    bool getExplode() {return exploding;}
    void chainExplosion();
    ~Block();
private:
    
    class Game* mGame;
    class MeshComponent* mc;
    class CollisionComponent* cc;
    bool exploding = false;
    int destroyRange = 50;
    int dynamicLimit = 2000;
};
