//
//  EnemyMove.hpp
//  Lab08
//
//  Created by Jiaqi Xu on 3/23/21.
//

#ifndef EnemyMove_hpp
#define EnemyMove_hpp

#include <stdio.h>
#include "VehicleMove.hpp"
#include <vector>
#include "Math.h"
#endif /* EnemyMove_hpp */
class EnemyMove : public VehicleMove
{
public:
    EnemyMove(class Actor* owner);
    void Update(float deltaTime) override;
    bool isInRadius();
private:
    std::vector<Vector3> points;
    int nextTargetPoint = 0;
};
