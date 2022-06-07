//
//  EnemyMove.cpp
//  Lab08
//
//  Created by Jiaqi Xu on 3/23/21.
//

#include "EnemyMove.hpp"
#include "Actor.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "CSVHelper.h"
#include "Game.h"
#include "HeightMap.hpp"
#include "Enemy.hpp"
EnemyMove::EnemyMove(class Actor* owner)
:VehicleMove(owner)
{
    //load enemy path
    int count = 0;
    std:: ifstream fileInput("Assets/HeightMap/Path.csv");
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
                Vector3 worldPos = mOwner->GetGame()->getHeightMap()->CellToWorld(oneIntLine[1], oneIntLine[2]);
                points.push_back(worldPos);
            }
            count++;
        }
    }
    mOwner->SetPosition(points[0]);
    nextTargetPoint = 1;
}

void EnemyMove::Update(float deltaTime){
    //enemy move
    setDirection(Direction::None);
    setPedal(false);
    float distance = (mOwner->GetGame()->getEnemy()->GetPosition()-points[nextTargetPoint]).Length();
    if(distance < 5){
        nextTargetPoint += 1;
        if(nextTargetPoint == 28){
            nextTargetPoint = 0;
        }
    }
    //accelarate if needed
    //get enemy forward vector
    Vector3 enemyForward =mOwner->GetGame()->getEnemy()->GetForward();
    //get unit vector from enemy to next target point
    Vector3 enemyToTarget = points[nextTargetPoint] - mOwner->GetGame()->getEnemy()->GetPosition();
    enemyToTarget.Normalize();

    float dot = Vector3::Dot(enemyForward, enemyToTarget);
    if(dot > 0.99){
        setPedal(true);
    }
    //turn
    Vector3 turn = Vector3::Cross(enemyForward, enemyToTarget);
    if(turn.z > 0){
        setDirection(Direction::Right);
    }
    else if(turn.z < 0){
        setDirection(Direction::Left);
    }
    

    VehicleMove::Update(deltaTime);
}
