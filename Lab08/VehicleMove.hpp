//
//  VehicleMove.hpp
//  Lab08
//
//  Created by Jiaqi Xu on 3/19/21.
//
#pragma once
#ifndef VehicleMove_hpp
#define VehicleMove_hpp

#include <stdio.h>
#include "Component.h"
#include "Math.h"
#include <vector>
#endif /* VehicleMove_hpp */
enum Direction
{
    Left,
    Right,
    None
};

class VehicleMove : public Component
{
public:
    VehicleMove(class Actor* owner);
    void Update(float deltaTime) override;
    void setDirection(Direction dir) {direction = dir;}
    void setPedal(bool pedal) {pedalPressed = pedal;}
    virtual void OnLapChange(int newLap) { }
    int getCurrLap() {return currentLap;}
    int getLastCheckPoint() {return LastCheckPoint;}
    float getDistanceFromCheckPoint();
    bool Final = false;
private:
    bool pedalPressed = false;
    Direction direction = Direction::None;
    Vector3 velocity=Vector3::Zero;
    float angularV;
    float accelerationTimer = 0.0f;
    float minLinearA = 1000.0f;
    float maxLinearA = 2500.0f;
    float accelerationRampTime = 1.5f;
    float angularAccelation = 5*Math::Pi;
    float drag = 0.9f;
    float dragPedalNotPressed = 0.975f;
    float angularDrag = 0.9f;
    std::vector<std::vector<int>> checkpoints;
    int currentLap = 0;
    int LastCheckPoint = -1;
    
};
