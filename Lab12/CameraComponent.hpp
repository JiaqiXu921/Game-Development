//
//  CameraComponent.hpp
//  Lab08
//
//  Created by Jiaqi Xu on 3/19/21.
//

#ifndef CameraComponent_hpp
#define CameraComponent_hpp

#include <stdio.h>
#include "Component.h"
#endif /* CameraComponent_hpp */
#include "Component.h"
#include "Math.h"
class CameraComponent : public Component
{
public:
    CameraComponent(class Actor* owner);
    void Update(float deltaTime) override;
    float GetPitchSpeed() const {return mPitchSpeed;}
    void SetPitchSpeed(float speed) {mPitchSpeed = speed;}
private:
    Vector3 cameraPos;
    Vector3 cameraVelocity;
    float mPitchAngle = 0.0f;
    float mPitchSpeed = 0.0f;
    float mWallRunAngle = 0.0f;
};
