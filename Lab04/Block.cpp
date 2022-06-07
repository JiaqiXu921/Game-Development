//
//  Block.cpp
//  Lab04
//
//  Created by Jiaqi Xu on 2/12/21.
//

#include "Block.hpp"

Block::Block(class Game* game)
:Actor(game)
{
    SpriteComponent* sC = new SpriteComponent(this);
    sc=sC;
    CollisionComponent* cC= new CollisionComponent(this);
    cc=cC;
    cc->SetSize(32.0f, 32.0f);
    mGame->AddBlock(this);
}


Block::~Block(){
    mGame->RemoveBlock(this);
}

void Block::changeTex(std::string& filename){
    sc->SetTexture(mGame->GetTexture(filename));
}
