//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"

// TODO
Game::Game(){
}

bool Game::Initialize(){
    bool result;
    previousTime=SDL_GetTicks();
    int video=SDL_Init(SDL_INIT_VIDEO);
    int audio=SDL_Init(SDL_INIT_AUDIO);
    //create a window
    myWindow=SDL_CreateWindow(        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        1024,                               // width, in pixels
        768,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );
    //create a renderer
    myRenderer=SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //check if video and audio successfully opened
    if(video==0 && audio==0 && myRenderer!=NULL && myWindow!=NULL){
        result=true;
    }
    else {
        result=false;
    }
    return result;
}

void Game::Shutdown(){
    SDL_DestroyRenderer(myRenderer);
    SDL_DestroyWindow(myWindow);
    SDL_Quit();
}

void Game::RunLoop(){
    while (endGame){
        bool exit=ProcessInput();
        if(exit==false){
            endGame=false;
        }
        UpdateGame();
        GenerateOutput();
    }
}

bool Game::ProcessInput(){
    bool result=true;
    SDL_Event event;
    //quit if the user clicks quit
    while (SDL_PollEvent(&event)) {
        if(event.type==SDL_QUIT){
            result=false;
        }
    }
    //quit if the user hits esc
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]){
        result=false;
    }
    //check if the user hits up or down
    const Uint8 *movement = SDL_GetKeyboardState(NULL);
    direction=0;
    if(movement[SDL_SCANCODE_UP]){
        direction=1;
    }
    else if(movement[SDL_SCANCODE_DOWN]){
        direction=-1;
    }
    return result;
}

void Game::UpdateGame(){
    int currentTime=SDL_GetTicks();
    //frame limiting
    while (currentTime-previousTime<16){
        currentTime=SDL_GetTicks();
    }
    //capping to max delta time
    float deltaTime=(float) (currentTime-previousTime)/1000;
    if (deltaTime>33){
        deltaTime=33;
    }
    //update previous time
   previousTime=currentTime;
    //check if the user holds down the UP or DOWN key
    if(direction==1){
        paddleC.y-=(int) (deltaTime*velocity);
        //fix paddle's position if it goes out of boundary
        if(paddleC.y<paddleH/2){
            paddleC.y=paddleH/2;
        }
    }
    else if(direction==-1){
        paddleC.y+=(int) (deltaTime*velocity);
        if(paddleC.y>768-paddleH/2){
            paddleC.y=768-paddleH/2;
        }
    }
    //if the ball's velocity become negative values, negate it
    if (ballV.x<0){
        ballV.x=-(int) (deltaTime*velocity);
    }
    else{
        ballV.x=(int) (deltaTime*velocity);
    }
    if(ballV.y<0){
        ballV.y=-(int) (deltaTime*velocity);
    }
    else{
        ballV.y=(int) (deltaTime*velocity);
    }
    ballC.x+=ballV.x;
    ballC.y+=ballV.y;
    //bounce back if the ball hits the top or bottom wall
    if(ballC.y>=windowH-thickness-thickness/2 || ballC.y<=thickness+thickness/2){
        ballV.y=-ballV.y;
    }
    //bounce back if the ball hits the right wall
    else if(ballC.x>=windowW-thickness-thickness/2){
        ballV.x=-ballV.x;
    }
    //check if the ball hits the paddle
    if(ballC.x<=thickness+thickness/2 && (ballC.y>=paddleC.y-paddleH/2 && ballC.y<=paddleC.y+paddleH/2)){
        ballV.x=-ballV.x;
    }
    //exit the game if the ball did not hit the paddle
    else if (ballC.x<=thickness+thickness/2&&(ballC.y>paddleC.y+paddleH/2 || ballC.y<paddleC.y-paddleH/2)){
        endGame=false;
    }
}

void Game::GenerateOutput(){
    //add background color
    SDL_SetRenderDrawColor(myRenderer, 0, 0, 255, 255);
    SDL_RenderClear(myRenderer);
    //add wall color
    SDL_SetRenderDrawColor(myRenderer, 255, 255, 255, 255);
    SDL_Rect top;
    SDL_Rect right;
    SDL_Rect bottom;
    SDL_Rect paddle;
    SDL_Rect ball;
    //create three walls
    top.x=0;
    top.y=0;
    top.w=windowW;
    top.h=thickness;
    right.x=windowW-thickness;
    right.y=0;
    right.w=thickness;
    right.h=windowH;
    bottom.x=0;
    bottom.y=windowH-thickness;
    bottom.w=windowW;
    bottom.h=thickness;
    SDL_RenderFillRect(myRenderer,&top);
    SDL_RenderFillRect(myRenderer,&right);
    SDL_RenderFillRect(myRenderer,&bottom);
    //create paddle
    paddle.w=thickness;
    paddle.h=paddleH;
    paddle.x=paddleC.x;
    paddle.y=paddleC.y-paddleH/2;
    SDL_RenderFillRect(myRenderer,&paddle);
    //create ball
    ball.x=ballC.x;
    ball.y=ballC.y;
    ball.w=thickness;
    ball.h=thickness;
    SDL_RenderFillRect(myRenderer,&ball);
    SDL_RenderPresent(myRenderer);
}
