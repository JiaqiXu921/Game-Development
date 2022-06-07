//
//  GoombaMove.cpp
//  Lab04
//
//  Created by Jiaqi Xu on 2/17/21.
//

#include "GoombaMove.hpp"
#include "Game.h"
GoombaMove::GoombaMove(Actor* owner)
:MoveComponent(owner)
,mOwner(owner)
{
    goomba=(Goomba*)mOwner;
}

void GoombaMove::Update(float deltatime){
    //goomba moves left initially with a speed of 100
    Vector2 goombaPos=goomba->GetPosition();
    goombaPos.x+=goombaSpeed*deltatime;
    goombaPos.y+=mYSpeed*deltatime;
    //destroy the goomba if it falls below the screen
    if (goombaPos.y >= 448){
           goomba->SetState(ActorState::Destroy);
       }
    goomba->SetPosition(goombaPos);
    
    CollSide collside;
    Vector2 offset=Vector2::Zero;
    //check collision with block
    for(Block* block:goomba->GetGame()->getBlocks()){
        bool intersect=goomba->getCC()->Intersect(block->getCC());
        if(intersect==true){
            collside=goomba->getCC()->GetMinOverlap(block->getCC(), offset);
            if(collside==CollSide::None){
                continue;
            }
            else if(collside==CollSide::Top){
                mYSpeed=0;
                goombaPos+=offset;
                mOwner->SetPosition(goombaPos);
            }
        }
    }
    //check collision with block
    for(Block* block:goomba->GetGame()->getBlocks()){
        bool intersect=goomba->getCC()->Intersect(block->getCC());
        if(intersect==true){
            collside=goomba->getCC()->GetMinOverlap(block->getCC(), offset);
            if(collside==CollSide::None){
                continue;
            }
            else if(collside==CollSide::Left||collside==CollSide::Right){
                goombaSpeed=-goombaSpeed;
                goombaPos+=offset;
                mOwner->SetPosition(goombaPos);
            }
        }
    }

    //check collision with goomba
    for(Goomba* oneGoomba:goomba->GetGame()->getGoombas()){
        if(goomba==oneGoomba){
            continue;
        }
        bool intersect=goomba->getCC()->Intersect(oneGoomba->getCC());
        if(intersect==true){
            collside=goomba->getCC()->GetMinOverlap(oneGoomba->getCC(), offset);
            if(collside==CollSide::None){
                continue;
            }
            else if(collside==CollSide::Left||collside==CollSide::Right){
                goombaSpeed=-goombaSpeed;
                goombaPos+=offset;
                mOwner->SetPosition(goombaPos);
            }
        }
    }
    mYSpeed += 2000.0f * deltatime;
    //check if goomba is stomped 
    if(goomba->getIsStomped()==true){
        goomba->getAsc()->SetAnimation("dead");
        goombaSpeed=0;
        mOwner->GetGame()->RemoveGoomba(goomba);
        timePassed+=deltatime;
    }
    if(timePassed>0.25){
       goomba->SetState(ActorState::Destroy);
    }

}
