//
//  Block.hpp
//  Lab09
//
//  Created by Jiaqi Xu on 3/26/21.
//

#ifndef Block_hpp
#define Block_hpp

#include <stdio.h>
#include "Actor.h"
#endif /* Block_hpp */
class Block : public Actor {
public:
    Block(class Game* game,Actor* parent);
    ~Block();
    class CollisionComponent* getCC() const {return cc;}
    bool GetIsMirror() const {return isMirror;}
    void SetIsMirror(bool ismirror) {isMirror=ismirror;}
    bool GetIsRotate() const {return isRotate;}
    void SetIsRotate(bool r) {isRotate=r;}
    void OnUpdate(float deltaTime) override;
private:
    bool isMirror=false;
    class Game* mGame;
    class MeshComponent* mc;
    class CollisionComponent* cc;
    bool isRotate = false;
    float ROTATION_ANGLE=Math::Pi/4;
};
