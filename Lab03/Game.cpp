//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//
#include "Game.h"
#include "Actor.h"
#include <algorithm>
#include "SpriteComponent.h"
#include "Random.h"
#include "Frog.hpp"
#include "Vehicle.hpp"
#include "Log.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
Game::Game(){
}

bool Game::Initialize(){
    Random::Init();
    bool initialized;
    int video=SDL_Init(SDL_INIT_VIDEO);
    int audio=SDL_Init(SDL_INIT_AUDIO);
    //create a window
    myWindow=SDL_CreateWindow(        "Frogger",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        WINDOW_WIDTH/2,                               // width, in pixels
        WINDOW_HEIGHT/2,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );
    //create a renderer
    myRenderer=SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(myRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    //check if video and audio successfully opened
    if(video==0 && audio==0 && myRenderer!=NULL && myWindow!=NULL){
        initialized=true;
    }
    else{
        initialized=false;
    }
    int flags=IMG_INIT_PNG;
    IMG_Init(flags);
    LoadData();
    previousTime=SDL_GetTicks();
    return initialized;
}

void Game::Shutdown(){
    IMG_Quit();
    UnLoadData();
    SDL_DestroyRenderer(myRenderer);
    SDL_DestroyWindow(myWindow);
    SDL_Quit();
}

void Game::RunLoop(){
    while (endGame){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput(){
    SDL_Event event;
    //quit if the user clicks quit
    while (SDL_PollEvent(&event)) {
        if(event.type==SDL_QUIT){
            endGame=false;
        }
    }
    //quit if the user hits esc
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]){
        endGame=false;
    }
    
    std::vector<Actor*> copyActors=mActor;
    for(Actor* copyActor:copyActors){
        copyActor->ProcessInput(state);
    }
}

void Game::UpdateGame(){
    int currentTime=SDL_GetTicks();
    //frame limiting
    while (currentTime-previousTime<FRAME_LIMITING){
        currentTime=SDL_GetTicks();
    }
    //capping to max delta time
    float deltaTime=(float) (currentTime-previousTime)/1000;
    if (deltaTime>MAX_DELTATIME){
        deltaTime=MAX_DELTATIME;
    }
    //update previous time
    previousTime=currentTime;
    
    //update actors
     std::vector<Actor*> copyActors=mActor;
     for(Actor* actor:copyActors){
         actor->Update(deltaTime);
     }
    //destroy the actors' whose state is destroy
    std::vector<Actor*> actorToDestroy;
    for(Actor* actor:mActor){
        if(actor->GetState()==ActorState::Destroy){
            actorToDestroy.push_back(actor);
        }
    }
    for(Actor* desActor:actorToDestroy){
        delete desActor;
    }
}

void Game::GenerateOutput(){
    //add background color
    SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 0);
    SDL_RenderClear(myRenderer);
    //check if sprite is visible
    for (auto sprite: mSprites){
        if(sprite->IsVisible()){
            sprite->Draw(myRenderer);
        }
    }
    SDL_RenderPresent(myRenderer);
}

void Game::AddActor(class Actor* actor){
    mActor.push_back(actor);
}

//find the actor and remove it from the vector
void Game::RemoveActor(class Actor* actor){
    std::vector<Actor*>::iterator toErase=std::find(mActor.begin(), mActor.end(), actor);
    if(toErase!=mActor.end()){
        mActor.erase(toErase);
    }
}

SDL_Texture* Game::GetTexture(std::string fileName){
    //check if the file is in the map
    if(myMap.find(fileName)!=myMap.end()){
        return myMap.find(fileName)->second;
    }
    //if the file is not in the map
    else{
        //create a image from the file
        SDL_Surface *image;
        image=IMG_Load(fileName.c_str());
        if(image==nullptr){
            SDL_Log("Failed to load", IMG_GetError());
        }
        else{
            //insert the image to the map
            SDL_Texture* texture = SDL_CreateTextureFromSurface(myRenderer, image);
            SDL_FreeSurface(image);
            myMap.insert(make_pair(fileName, texture));
            return myMap.find(fileName)->second;
        }
    }
    return nullptr;
}

void Game::LoadData(){
    Actor* actor = new Actor(this);
    Vector2 position(WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
    actor->SetPosition(position);
    SpriteComponent* sc = new SpriteComponent(actor);
    sc->SetTexture(GetTexture("Assets/Background.png"));
    std:: ifstream fileInput("Assets/Level.txt");
    std::string name;
    int numLines=0;
    //if the file is successfully opened
    if(fileInput.is_open()){
        //get lines until the end
        while(!fileInput.eof()){
            std::string line;
            std::getline(fileInput, line);
            //if the line is not an empty line
            if(!line.empty()){
                for (int i=0;i<TOTAL_ROW;i++){
                    if(line.at(i)=='F'){
                        Frog* frog=new Frog(this);
                        myFrog=frog;
                        Vector2 frogP(BLOCK_SIZE+i*BLOCK_SIZE,TOP_LEFT_Y+numLines*BLOCK_SIZE);
                        frog->SetPosition(frogP);
                        SpriteComponent* scF = new SpriteComponent(frog);
                        scF->SetTexture(GetTexture("Assets/Frog.png"));
                    }
                    else if(line.at(i)=='G'){
                        goal=new Actor(this);
                        Vector2 goalP(BLOCK_SIZE+i*BLOCK_SIZE,TOP_LEFT_Y+numLines*BLOCK_SIZE);
                        goal->SetPosition(goalP);
                    }
                    else if (line.at(i)=='A'){
                        Vehicle* carA=new Vehicle(this);
                        carA->setRowNum(numLines);
                        Vector2 carAP(BLOCK_SIZE+i*BLOCK_SIZE,TOP_LEFT_Y+numLines*BLOCK_SIZE);
                        carA->SetPosition(carAP);
                        SpriteComponent* scF = new SpriteComponent(carA);
                        scF->SetTexture(GetTexture("Assets/CarA.png"));
                    }
                    else if (line.at(i)=='B'){
                        Vehicle* carB=new Vehicle(this);
                        carB->setRowNum(numLines);
                        Vector2 carBP(BLOCK_SIZE+i*BLOCK_SIZE,TOP_LEFT_Y+numLines*BLOCK_SIZE);
                        carB->SetPosition(carBP);
                        SpriteComponent* scF = new SpriteComponent(carB);
                        scF->SetTexture(GetTexture("Assets/CarB.png"));
                    }
                    else if (line.at(i)=='C'){
                        Vehicle* carC=new Vehicle(this);
                        carC->setRowNum(numLines);
                        Vector2 carCP(BLOCK_SIZE+i*BLOCK_SIZE,TOP_LEFT_Y+numLines*BLOCK_SIZE);
                        carC->SetPosition(carCP);
                        SpriteComponent* scF = new SpriteComponent(carC);
                        scF->SetTexture(GetTexture("Assets/CarC.png"));
                    }
                    else if (line.at(i)=='D'){
                        Vehicle* carD=new Vehicle(this);
                        carD->setRowNum(numLines);
                        Vector2 carDP(BLOCK_SIZE+i*BLOCK_SIZE,TOP_LEFT_Y+numLines*BLOCK_SIZE);
                        carD->SetPosition(carDP);
                        SpriteComponent* scF = new SpriteComponent(carD);
                        scF->SetTexture(GetTexture("Assets/CarD.png"));
                       
                    }
                    else if (line.at(i)=='T'){
                        Vehicle* carT=new Vehicle(this);
                        carT->setRowNum(numLines);
                        Vector2 carTP(BLOCK_SIZE+i*BLOCK_SIZE,TOP_LEFT_Y+numLines*BLOCK_SIZE);
                        carT->SetPosition(carTP);
                        SpriteComponent* scF = new SpriteComponent(carT);
                        scF->SetTexture(GetTexture("Assets/Truck.png"));
                        mVehicles.push_back(carT);
                    }
                    else if (line.at(i)=='X'){
                        Log* logX=new Log(this);
                        logX->setRowNum(numLines);
                        Vector2 logXP(BLOCK_SIZE+i*BLOCK_SIZE,TOP_LEFT_Y+numLines*BLOCK_SIZE);
                        logX->SetPosition(logXP);
                        SpriteComponent* scF = new SpriteComponent(logX);
                        scF->SetTexture(GetTexture("Assets/logX.png"));
                        logX->getCC()->SetSize(LOGX_W, LOG_H);
                    }
                    else if (line.at(i)=='Y'){
                        Log* logY=new Log(this);
                        logY->setRowNum(numLines);
                        Vector2 logYP(BLOCK_SIZE+i*BLOCK_SIZE,TOP_LEFT_Y+numLines*BLOCK_SIZE);
                        logY->SetPosition(logYP);
                        SpriteComponent* scF = new SpriteComponent(logY);
                        scF->SetTexture(GetTexture("Assets/logY.png"));
                        logY->getCC()->SetSize(LOGY_W, LOG_H);
                    }
                    else if (line.at(i)=='Z'){
                        Log* logZ=new Log(this);
                        logZ->setRowNum(numLines);
                        Vector2 logZP(BLOCK_SIZE+i*BLOCK_SIZE,TOP_LEFT_Y+numLines*BLOCK_SIZE);
                        logZ->SetPosition(logZP);
                        SpriteComponent* scF = new SpriteComponent(logZ);
                        scF->SetTexture(GetTexture("Assets/logZ.png"));
                        logZ->getCC()->SetSize(LOGZ_W, LOG_H);
                    }
                }
            }
            numLines++;
        }
    }
}

void Game::UnLoadData(){
    //delete the elements in the vector
    while(!mActor.empty()){
        delete mActor.back();
    }
    for(auto ite=myMap.begin();ite!=myMap.end();++ite){
        SDL_DestroyTexture(ite->second);
    }
    myMap.clear();
}

void Game::AddSprite(SpriteComponent* spriteToAdd){
    mSprites.push_back(spriteToAdd);
    //sort sprites 
    std::sort(mSprites.begin(), mSprites.end(),[](SpriteComponent* a, SpriteComponent* b) {
    return a->GetDrawOrder() < b->GetDrawOrder();
    });
}

void Game::RemoveSprite(SpriteComponent* spriteToRemove){
    std::vector<SpriteComponent*>::iterator sctoErase=std::find(mSprites.begin(), mSprites.end(), spriteToRemove);
    if(sctoErase!=mSprites.end()){
        mSprites.erase(sctoErase);
    }
}

void Game::AddVehicle(Vehicle* vehicle){
    mVehicles.push_back(vehicle);
}

void Game::RemoveVehicle(Vehicle* vehicle){
    std::vector<Vehicle*>::iterator toErase=std::find(mVehicles.begin(), mVehicles.end(), vehicle);
    if(toErase!=mVehicles.end()){
        mVehicles.erase(toErase);
    }
}

void Game::AddLog(Log* log){
    mLogs.push_back(log);
}

void Game::RemoveLog(Log* log){
    std::vector<Log*>::iterator toErase=std::find(mLogs.begin(), mLogs.end(), log);
    if(toErase!=mLogs.end()){
        mLogs.erase(toErase);
    }
}
