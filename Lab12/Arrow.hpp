//
//  Arrow.hpp
//  Lab11
//
//  Created by Jiaqi Xu on 4/12/21.
//

#ifndef Arrow_hpp
#define Arrow_hpp

#include <stdio.h>
#include "Actor.h"
#endif /* Arrow_hpp */
class Arrow : public Actor {
public:
    Arrow(class Game* game, Actor* parent);
    virtual void OnUpdate(float deltaTime) override;

private:
    class MeshComponent* mc;
};
