//
//  Main.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//
#include <iostream>
#include <string>
#include "Game.h"

int main(int argc, char** argv)
{
    Game newGame;
    bool result=newGame.Initialize();
    if(result==true){
        newGame.RunLoop();
    }
    newGame.Shutdown();
	return 0;
}
