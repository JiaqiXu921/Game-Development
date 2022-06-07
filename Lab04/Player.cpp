//
//  Player.cpp
//  Lab04
//
//  Created by Jiaqi Xu on 2/12/21.
//

#include "Player.hpp"
Player::Player(class Game* game)
:Actor(game)
{
    AnimatedSprite* asC = new AnimatedSprite(this);
    asc=asC;
    asc->setDrawOrder(150);
    
    std::vector<SDL_Texture*>idle{GetGame()->GetTexture("Assets/Mario/Idle.png")};
    std::vector<SDL_Texture*>dead{GetGame()->GetTexture("Assets/Mario/Dead.png")};
    std::vector<SDL_Texture*>jumpLeft{GetGame()->GetTexture("Assets/Mario/JumpLeft.png")};
    std::vector<SDL_Texture*>jumpRight{GetGame()->GetTexture("Assets/Mario/JumpRight.png")};
    std::vector<SDL_Texture*>runLeft{GetGame()->GetTexture("Assets/Mario/RunLeft0.png"),
        GetGame()->GetTexture("Assets/Mario/RunLeft1.png"),
        GetGame()->GetTexture("Assets/Mario/RunLeft2.png")
    };
    std::vector<SDL_Texture*>runRight{GetGame()->GetTexture("Assets/Mario/RunRight0.png"),
        GetGame()->GetTexture("Assets/Mario/RunRight1.png"),
        GetGame()->GetTexture("Assets/Mario/RunRight2.png")
    };
    asc->AddAnimation("idle", idle);
    asc->AddAnimation("dead", dead);
    asc->AddAnimation("jumpLeft", jumpLeft);
    asc->AddAnimation("jumpRight", jumpRight);
    asc->AddAnimation("runLeft", runLeft);
    asc->AddAnimation("runRight", runRight);
    asc->SetAnimation("idle");

    CollisionComponent* cC=new CollisionComponent(this);
    cc=cC;
    cc->SetSize(32.0f, 32.0f);
    PlayerMove* pM=new PlayerMove(this);
    pm=pM;
}
