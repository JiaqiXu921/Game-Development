//
//  SideBlock.hpp
//  Lab07
//
//  Created by Jiaqi Xu on 3/15/21.
//

#ifndef SideBlock_hpp
#define SideBlock_hpp

#include <stdio.h>
#include "Actor.h"
#endif /* SideBlock_hpp */
class SideBlock : public Actor {
public:
    SideBlock(class Game* game);
    void OnUpdate(float deltaTime) override;
    class MeshComponent* getMC() { return mc; }
private:
    
    class Game* mGame;
    class MeshComponent* mc;
};
