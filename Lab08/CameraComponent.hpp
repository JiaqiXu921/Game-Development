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
//class CameraComponent : public Component
//{
//public:
//    CameraComponent(class Actor* owner);
//    void Update(float deltaTime) override;
//    
//private:
//    
//};
#include "Component.h"
#include "Math.h"
class CameraComponent : public Component
{
public:
    CameraComponent(class Actor* owner);
    void Update(float deltaTime) override;
    void SnapToIdeal();
    
private:
    float hDist=60.0f;
    float offset = 50.0f;
    float springConst=256.0f;
    float dampConst = 2.0f*Math::Sqrt(springConst);
    Vector3 cameraPos;
    Vector3 cameraVelocity;
};
