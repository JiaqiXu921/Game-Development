//
//  HUD.cpp
//  Lab12
//
//  Created by Jiaqi Xu on 4/16/21.
//

#include "HUD.hpp"
#include "Font.h"
#include "Actor.h"
#include "Texture.h"
#include "Shader.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Game.h"
#include "Player.hpp"
#include "PlayerMove.hpp"
#include "CheckPoint.hpp"
#include "Renderer.h"
#include "SecurityCamera.hpp"
HUD::HUD(Actor* owner)
:UIComponent(owner)
{
    mFont = new Font();
    mFont->Load("Assets/Inconsolata-Regular.ttf");
    mTimerText = mFont->RenderText("00:00.00");
    std::string num = std::to_string(mOwner->GetGame()->totalCoin);
    std::string coinT = num+"/55";
    mCoinText = mFont->RenderText(coinT);
    
    mRadarBack = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Radar.png");
    mArrow = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/RadarArrow.png");
    mBlip = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Blip.png");

}

HUD::~HUD(){
    mFont->Unload();
    delete mFont;
}

void HUD::Draw(Shader* shader){
    DrawTexture(shader, mTimerText, Vector2(-420.0f, -325.0f));
    DrawTexture(shader, mCoinText, Vector2(-443.0f, -290.0f));
    if(mOwner->GetGame()->draw){
        DrawTexture(shader, mOwner->GetGame()->mCheckPointText, Vector2::Zero);
    }
    
    DrawTexture(shader, mRadarBack, Vector2(410.0f, -280.0f));
    DrawTexture(shader, mArrow, Vector2(410.0f, -280.0f));
    
    Vector2 player2D = Vector2(mOwner->GetGame()->getPlayer()->GetPosition().y,mOwner->GetGame()->getPlayer()->GetPosition().x);
    float angle = Math::Atan2(mOwner->GetGame()->getPlayer()->GetForward().y, mOwner->GetGame()->getPlayer()->GetForward().x);
    for(SecurityCamera* sc: mOwner->GetGame()->getCameras()){
        Vector2 camera2D = Vector2(sc->GetPosition().y,sc->GetPosition().x);
        Vector2 cameraToPlayer = camera2D - player2D;
        if(cameraToPlayer.Length() <= 1500.0f){
            cameraToPlayer.x = cameraToPlayer.x/1500.0f;
            cameraToPlayer.y = cameraToPlayer.y/1500.0f;
            cameraToPlayer *= 92;
            Vector2 rotationVector = Vector2::Transform(cameraToPlayer, Matrix3::CreateRotation(angle));
            
            Vector2 objectFwd = Vector2(sc->GetWorldForward().y,sc->GetWorldForward().x);
            Vector2 objectRotation = Vector2::Transform(objectFwd, Matrix3::CreateRotation(angle));
            float cameraAngle = Math::Atan2(objectRotation.y, objectRotation.x);
            
            DrawTexture(shader, mBlip,Vector2(rotationVector.x+410.0f,rotationVector.y-280.0f),1.0f, cameraAngle);
        }
    }
}

void HUD::Update(float deltaTime){
    std::string timeString="";
    mOwner->GetGame()->time += deltaTime;
    mTimerText->Unload();
    delete mTimerText;
    int minutes = (int)mOwner->GetGame()->time/60;
    float seconds = mOwner->GetGame()->time - 60 * minutes;
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << seconds;
    std::string roundSeconds = stream.str();
    if(minutes < 10){
        timeString += "0"+std::to_string(minutes)+":";
    }
    else{
        timeString += std::to_string(minutes)+":";
    }
    if(seconds<10.0f){
        timeString += "0"+roundSeconds;
    }
    else{
        timeString += roundSeconds;
    }
    mTimerText = mFont->RenderText(timeString);
}

void HUD::CoinCollected(){
    std::string coinText="";
    mOwner->GetGame()->totalCoin -= 1;
    if(mOwner->GetGame()->totalCoin < 10){
        coinText += "0" + std::to_string(mOwner->GetGame()->totalCoin) + "/55";
    }
    else{
        coinText += std::to_string(mOwner->GetGame()->totalCoin) + "/55";
    }
    mCoinText = mFont->RenderText(coinText);
}

void HUD::updateText(){
    std::string checkpointText = mOwner->GetGame()->getPlayer()->getPM()->GetNextCheckPoint()->GetCheckPointText();
    mOwner->GetGame()->mCheckPointText = mFont->RenderText(checkpointText);
    mOwner->GetGame()->draw = true;
}
