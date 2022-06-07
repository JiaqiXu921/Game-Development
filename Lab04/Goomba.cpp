//
//  Goomba.cpp
//  Lab04
//
//  Created by Jiaqi Xu on 2/17/21.
//

#include "Goomba.hpp"
Goomba::Goomba(class Game* game)
:Actor(game)
{
    AnimatedSprite* asC = new AnimatedSprite(this);
    asc=asC;
    asc->setDrawOrder(150);
    
    std::vector<SDL_Texture*> walkAnim{
        GetGame()->GetTexture("Assets/Goomba/Walk0.png"),
        GetGame()->GetTexture("Assets/Goomba/Walk1.png")
    };
    std::vector<SDL_Texture*>deadAnim{
        GetGame()->GetTexture("Assets/Goomba/Dead.png")
        
    };
    asc->AddAnimation("walk", walkAnim);
    asc->SetAnimation("walk");
    asc->AddAnimation("dead", deadAnim);
    CollisionComponent* cC=new CollisionComponent(this);
    cc=cC;
    GoombaMove* gM = new GoombaMove(this);
    gm = gM;
    
    cc->SetSize(32.0f, 32.0f);
    mGame->AddGoomba(this);
    
}

Goomba::~Goomba(){
    mGame->RemoveGoomba(this);
}
