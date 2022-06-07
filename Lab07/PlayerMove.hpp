//
//  PlayerMove.hpp
//  Lab07
//
//  Created by Jiaqi Xu on 3/15/21.
//

#ifndef PlayerMove_hpp
#define PlayerMove_hpp

#include <stdio.h>
#include "MoveComponent.h"
#include "Math.h"
#endif /* PlayerMove_hpp */
class PlayerMove : public MoveComponent{
public:
    PlayerMove(class Actor* owner);
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    float getTimeMultiplier() {return timeMultiplier;}
private:
    class Actor* mOwner;
    class Player* player;
    Vector3 velocity;
    int sideblockX = 3000;
    int blockX = 3000;
    int colorIndex=2;
    int fileNum = 3;
    bool random=false;
    bool spacePressed=false;
    float timeMultiplier = 1.0f;
    float timer=0.0f;
    float BULLET_TIMER = 10;
    int Y_LIMIT = 195;
    int Z_LIMIT = 225;
    float movementSpeed=300;
    int totalFiles = 20;
    float generateBound = 3000;
    int totalColors = 5;
    int sideBlockSize = 500;
    int blockSize = 1000;
    int defaultSpeed=400;
};
