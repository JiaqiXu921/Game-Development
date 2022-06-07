//
//  SecurityCone.hpp
//  Lab11
//
//  Created by Jiaqi Xu on 4/13/21.
//

#ifndef SecurityCone_hpp
#define SecurityCone_hpp

#include <stdio.h>
#include "Actor.h"
#include <string>
#endif /* SecurityCone_hpp */
class SecurityCone : public Actor {
public:
    SecurityCone(class Game* game,Actor* parent);
    virtual void OnUpdate(float deltaTime) override;
    class MeshComponent* getMC() const {return mc;}
    std::string getColor() const {return color;}
    int detected=-1;
private:
    class MeshComponent* mc;
    float timer=2.0f;
    std::string color = "";
    
    bool detectPlayOnce = false;
    float DIE_TIMER = 0.5f;
    float CONE_HEIGHT = 300;
    float HALF_ANGLE = Math::Pi/6;
};

