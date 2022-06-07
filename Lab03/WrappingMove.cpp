//
//  WrappingMove.cpp
//  Lab03
//
//  Created by Jiaqi Xu on 2/9/21.
//

#include "WrappingMove.hpp"
WrappingMove::WrappingMove(class Actor* owner)
:MoveComponent(owner)
{
    
}

void WrappingMove::Update(float deltaTime) {
    Vector2 currPos=mOwner->GetPosition();
    //set the direction to be <-1,0> or <1,0> depending on rows
    if(mOwner->getRowNum()%2==0){
        Vector2 dir(1,0);
        direction=dir;
    }
    else{
        Vector2 dir(-1,0);
        direction=dir;
    }
    //wrap around actors' positions
    if(currPos.x<0){
        currPos.x=WINDOW_WIDTH;
    }
    else if(currPos.x>WINDOW_WIDTH){
        currPos.x=0;
    }
    //update position
     currPos+=direction*this->GetForwardSpeed()*deltaTime;
    mOwner->SetPosition(currPos);
}
