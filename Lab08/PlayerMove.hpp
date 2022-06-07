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
#include "VehicleMove.hpp"
#endif /* PlayerMove_hpp */
class PlayerMove : public VehicleMove{
public:
    PlayerMove(class Actor* owner);
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void OnLapChange(int newLap) override;
private:
    class Actor* mOwner;
    class Player* player;
    Vector3 velocity;
    bool random=false;
    bool spacePressed=false;
    float movementSpeed=300;
    int defaultSpeed=400;
};
