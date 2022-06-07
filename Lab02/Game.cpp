//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//
#include "Ship.hpp"
#include "Game.h"
#include "Actor.h"
#include <algorithm>
#include "SpriteComponent.h"
#include "Random.h"
#include "Asteroid.hpp"
Game::Game(){
}

bool Game::Initialize(){
    Random::Init();
    bool initialized;
    int video=SDL_Init(SDL_INIT_VIDEO);
    int audio=SDL_Init(SDL_INIT_AUDIO);
    //create a window
    myWindow=SDL_CreateWindow(        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        WINDOW_WIDTH,                               // width, in pixels
        WINDOW_HEIGHT,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );
    //create a renderer
    myRenderer=SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
    //load background
    Actor* test4 = new Actor(this);
    Vector2 position4(WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
    test4->SetPosition(position4);
    SpriteComponent* sc4 = new SpriteComponent(test4,UPDATE_ORDER);
    sc4->SetTexture(GetTexture("Assets/Stars.png"));
    //load ship
    Ship* ship1 = new Ship(this);
    Vector2 position(WINDOW_WIDTH/2,WINDOW_HEIGHT/2);
    ship1->SetPosition(position);
    //create 10 new asteroids
    for (int i=0;i<NUM_ASTEROID;i++){
        new Asteroid(this);
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

void Game::AddAsteroid(class Asteroid* asteroid){
    mAsteroids.push_back(asteroid);
}

void Game::RemoveAsteroid(class Asteroid* asteroid){
    std::vector<Asteroid*>::iterator toErase=std::find(mAsteroids.begin(), mAsteroids.end(), asteroid);
    if(toErase!=mAsteroids.end()){
        mAsteroids.erase(toErase);
    }
}

