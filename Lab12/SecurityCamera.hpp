//
//  SecurityCamera.hpp
//  Lab11
//
//  Created by Jiaqi Xu on 4/13/21.
//

#ifndef SecurityCamera_hpp
#define SecurityCamera_hpp

#include <stdio.h>
#include "Actor.h"
#endif /* SecurityCamera_hpp */
class SecurityCamera : public Actor {
public:
    SecurityCamera(class Game* game,Actor* parent);
    ~SecurityCamera();
    void SetStartQ(Quaternion q) {startQ = q;}
    void SetEndQ(Quaternion q) {endQ = q;}
    void SetInterpTime(float time) {interpTime = time;}
    void SetPauseTime(float time) {pauseTime = time;}
    virtual void OnUpdate(float deltaTime) override;
private:
    class MeshComponent* mc;
    class SecurityCone* sc;
    Quaternion startQ;
    Quaternion endQ;
    float interpTime;
    float pauseTime=0.0f;
    float timer=0.0f;
    float pauseTimer=pauseTime;
    int lastCount;
    bool paused = false;
    int startRotate=-1;
    int stopRotate=-1;
    bool rotatePlayOnce = false;
    bool stopPlayOnce = false;
    float SOUND_FAR_LIMIT = 1500.0f;
    float SOUND_NEAR_LIMIT = 500.0f;
    int MIN_VOL = 0;
    int MAX_VOL = 128;
    float INTERP_K = -0.128f;
    float INTERP_B = 192.0f;
};
