//
//  SecurityCamera.cpp
//  Lab11
//
//  Created by Jiaqi Xu on 4/13/21.
//

#include "SecurityCamera.hpp"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "SecurityCone.hpp"
#include <iostream>
#include "Player.hpp"
SecurityCamera::SecurityCamera(class Game* game,Actor* parent)
:Actor(game,parent)
{
    mc = new MeshComponent(this);
    mc->SetMesh(game->GetRenderer()->GetMesh("Assets/Camera.gpmesh"));
    sc = new SecurityCone(game,this);
    game->AddCamera(this);
}

SecurityCamera::~SecurityCamera(){
    if(startRotate != -1){
        Mix_HaltChannel(startRotate);
    }
    GetGame()->RemoveCamera(this);
    delete sc;
}

void SecurityCamera::OnUpdate(float deltaTime){
    //change
    if(sc->detected != -1){
        if(startRotate!=-1){
            Mix_Pause(startRotate);
        }
    }
    else{
        Mix_Resume(startRotate);
    }
    Vector3 playerToCamera =GetGame()->getPlayer()->GetPosition()-this->GetPosition();
    Quaternion q;
    int count = (int)(timer/interpTime);
    float time = timer - count * interpTime;
    if(lastCount != count){
        paused = true;
    }
    if(paused == false && sc->getColor() == "white"){
        stopPlayOnce = false;
        timer+=deltaTime;
        if(count % 2 == 0){
            if(rotatePlayOnce == false){
                Mix_Chunk* rotate=GetGame()->GetSound("Assets/Sounds/CameraMotor.wav");
                startRotate = Mix_PlayChannel(Mix_GroupAvailable(1), rotate, 0);
                if(playerToCamera.Length()>SOUND_FAR_LIMIT){
                    Mix_Volume(startRotate, MIN_VOL);
                }
                if(playerToCamera.Length()<SOUND_NEAR_LIMIT){
                    Mix_Volume(startRotate, MAX_VOL);
                }
                if(playerToCamera.Length()>SOUND_NEAR_LIMIT && playerToCamera.Length()<SOUND_FAR_LIMIT){
                    int volume = (int) (-0.128*playerToCamera.Length()+192);
                    Mix_Volume(startRotate, volume);
                }
                rotatePlayOnce=true;
            }
            q = Quaternion::Slerp(startQ, endQ, time/interpTime);
            this->SetQuaternion(q);
        }
        else if(count % 2 == 1){
            if(rotatePlayOnce == false){
                Mix_Chunk* rotate=GetGame()->GetSound("Assets/Sounds/CameraMotor.wav");
                startRotate = Mix_PlayChannel(Mix_GroupAvailable(1), rotate, 0);
                if(playerToCamera.Length()>SOUND_FAR_LIMIT){
                    Mix_Volume(startRotate, MIN_VOL);
                }
                if(playerToCamera.Length()<SOUND_NEAR_LIMIT){
                    Mix_Volume(startRotate, MAX_VOL);
                }
                if(playerToCamera.Length()>SOUND_NEAR_LIMIT && playerToCamera.Length()<SOUND_FAR_LIMIT){
                    int volume = (int) (INTERP_K*playerToCamera.Length()+INTERP_B);
                    Mix_Volume(startRotate, volume);
                }
                rotatePlayOnce=true;
            }
            q = Quaternion::Slerp(endQ, startQ, time/interpTime);
            this->SetQuaternion(q);
        }
        if(playerToCamera.Length()>SOUND_FAR_LIMIT){
            Mix_Volume(startRotate, MIN_VOL);
        }
        if(playerToCamera.Length()<SOUND_NEAR_LIMIT){
            Mix_Volume(startRotate, MAX_VOL);
        }
        if(playerToCamera.Length()>SOUND_NEAR_LIMIT && playerToCamera.Length()<SOUND_FAR_LIMIT){
            int volume = (int) (INTERP_K*playerToCamera.Length()+INTERP_B);
            Mix_Volume(startRotate, volume);
        }
    }
    if(paused){
        rotatePlayOnce = false;
        if(startRotate!=-1){
            Mix_HaltChannel(startRotate);
        }
        if(stopPlayOnce == false){
            Mix_Chunk* stop=GetGame()->GetSound("Assets/Sounds/CameraMotorStop.wav");
            stopRotate = Mix_PlayChannel(Mix_GroupAvailable(1), stop, 0);
            if(playerToCamera.Length()>SOUND_FAR_LIMIT){
                Mix_Volume(stopRotate, MIN_VOL);
            }
            if(playerToCamera.Length()<SOUND_NEAR_LIMIT){
                Mix_Volume(stopRotate, MAX_VOL);
            }
            if(playerToCamera.Length()>SOUND_NEAR_LIMIT && playerToCamera.Length()<SOUND_FAR_LIMIT){
                int volume = (int) (INTERP_K*playerToCamera.Length()+INTERP_B);
                Mix_Volume(stopRotate, volume);
            }
            stopPlayOnce=true;
        }
        startRotate = -1;
        pauseTimer -= deltaTime;
        if(pauseTimer<0.0f){
            pauseTimer = pauseTime;
            paused = false;
        }
    }
    lastCount = count;
}
