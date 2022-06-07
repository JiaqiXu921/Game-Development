//
//  LaserComponent.hpp
//  Lab10
//
//  Created by Jiaqi Xu on 4/5/21.
//

#ifndef LaserComponent_hpp
#define LaserComponent_hpp

#include <stdio.h>
#include "MeshComponent.h"
#include "SegmentCast.h"
#include "Math.h"
#include <vector>
#endif /* LaserComponent_hpp */
class LaserComponent : public MeshComponent
{
public:
    LaserComponent(class Actor* owner, bool usesAlpha = false);
    virtual void Draw(class Shader* shader) override;
    virtual void Update(float deltaTime) override;
    Matrix4 helper(LineSegment lineSeg);
private:
    std::vector<LineSegment> lineSegments;
    float MIDPOINT = 0.5f;
};
