//
//  VehicleMove.cpp
//  Lab08
//
//  Created by Jiaqi Xu on 3/19/21.
//

#include "VehicleMove.hpp"
#include "Actor.h"
#include "HeightMap.hpp"
#include "Game.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "CSVHelper.h"
VehicleMove::VehicleMove(class Actor* owner)
:Component(owner, 50)
{
    int count = 0;
    std:: ifstream fileInput("Assets/HeightMap/Checkpoints.csv");
    if(fileInput.is_open()){
        while(!fileInput.eof()){
            std::string line;
            std::getline(fileInput, line);
            std::vector<int> oneIntLine;
            std::vector<std::string> oneStringLine;
            if(line.size()!=0 && count != 0){
                oneStringLine=CSVHelper::Split(line);
                for(unsigned int i=0;i<oneStringLine.size();i++){
                    oneIntLine.push_back(std::atoi(oneStringLine[i].c_str()));
                }
                checkpoints.push_back(oneIntLine);
            }
            count++;
        }
    }
}

void VehicleMove::Update(float deltaTime)
{
    Vector3 position = mOwner->GetPosition();
    float angle = mOwner->GetRotation();
    if(pedalPressed){
        accelerationTimer += deltaTime;
        float aMagnitude=0.0f;
        if(accelerationTimer>accelerationRampTime){
            aMagnitude=Math::Lerp(minLinearA, maxLinearA, 1);
        }
        else{
            aMagnitude=Math::Lerp(minLinearA, maxLinearA, accelerationTimer/accelerationRampTime);
        }
        velocity += mOwner->GetForward() * aMagnitude * deltaTime;
    }
    else{
        accelerationTimer = 0.0f;
    }
    position += velocity * deltaTime;

    if(pedalPressed){
        velocity *= drag;
    }
    else{
        velocity *= dragPedalNotPressed;
    }

    if(direction == Direction::Left || direction == Direction::Right){
        int turningFactor = 0;
        if(direction == Direction::Left){
            turningFactor = -1;
        }
        else{
            turningFactor = 1;
        }
        angularV += turningFactor * angularAccelation * deltaTime;
    }
    angle += angularV * deltaTime;
    angularV *= angularDrag;
    mOwner->SetPosition(position);
    mOwner->SetRotation(angle);
    
    if(mOwner->GetGame()->getHeightMap()->IsOnTrack(position.x, position.y)){
        Vector3 newPosition = position;
        float newZ = mOwner->GetGame()->getHeightMap()->GetHeight(position.x, position.y);
        newPosition.z = Math::Lerp(position.z, newZ, 0.1f);
        mOwner->SetPosition(newPosition);
    }
    
    //lap
    int currCheckPoint = LastCheckPoint+1;
    if(currCheckPoint == 8){
        currCheckPoint = 0;
    }
    
    Vector2 currCell = mOwner->GetGame()->getHeightMap()->WorldToCell(mOwner->GetPosition().x, mOwner->GetPosition().y);
    Vector2 minCell(checkpoints[currCheckPoint][1],checkpoints[currCheckPoint][3]);
    Vector2 maxCell(checkpoints[currCheckPoint][2],checkpoints[currCheckPoint][4]);
    
    if((minCell.x == maxCell.x && currCell.y >= minCell.y && minCell.x == static_cast<int>(currCell.x) && currCell.y <= maxCell.y) || (minCell.y == maxCell.y && currCell.x >= minCell.x && minCell.y == static_cast<int>(currCell.y) && currCell.x <= maxCell.x)){
        LastCheckPoint +=1;
        if(LastCheckPoint == 7){
            LastCheckPoint = -1;
        }
        
        if(LastCheckPoint == 0){
            currentLap += 1;
            if(currentLap==4){
                Final=true;
            }
        }
        OnLapChange(currentLap);
    }
}

float VehicleMove::getDistanceFromCheckPoint(){
    Vector2 currentPosition = mOwner->GetGame()->getHeightMap()->WorldToCell(mOwner->GetPosition().x, mOwner->GetPosition().y);
    if(LastCheckPoint != (int)checkpoints.size()-1){
        Vector2 checkPoint(checkpoints[LastCheckPoint+1][1],checkpoints[LastCheckPoint+1][3]);
        return (checkPoint-currentPosition).Length();
    }
    else{
        Vector2 checkPoint(checkpoints[0][1],checkpoints[0][3]);
        return (checkPoint-currentPosition).Length();
    }
}
