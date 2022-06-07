//
//  PlayerMove.hpp
//  Lab07
//
//  Created by Jiaqi Xu on 3/15/21.
//

#ifndef PlayerMove_hpp
#define PlayerMove_hpp

#include <stdio.h>
#include "MoveComponent.h"
#include "Math.h"
#include "CollisionComponent.h"
#endif /* PlayerMove_hpp */

class PlayerMove : public MoveComponent{
public:
    enum MoveState
    {
        OnGround,
        Jump,
        Falling,
        WallClimb,
        WallRun
    };
    ~PlayerMove();
    PlayerMove(class Actor* owner);
    void Update(float deltaTime) override;
    void ProcessInput(const Uint8* keyState) override;
    void ChangeState(MoveState state) {mCurrentState = state;}
    void PhysicsUpdate(float deltaTime);
    void AddForce(const Vector3& force);
    MoveState getCurrentState() const {return mCurrentState;}
    Vector3 getNormal(CollSide collside);
    CollSide getWallRunSide() const {return wallRunSide; }
    class CheckPoint* GetNextCheckPoint() const {return nextCheckPoint;}
protected:
    void UpdateOnGround(float deltaTime);
    void UpdateJump(float deltaTime);
    void UpdateFalling(float deltaTime);
    void UpdateWallClimb(float deltaTime);
    void UpdateWallRun(float deltaTime);
    bool CanWallClimb(CollSide collside);
    bool CanWallRun(CollSide collside);
    CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
    void FixXYVelocity();

private:
    class Actor* mOwner;
    MoveState mCurrentState;
    float maxMouseMovement=500.0f;
    float radPerSec=10.0f;
    bool spacePressed = false;
    Vector3 mVelocity;
    Vector3 mAcceleration;
    Vector3 mPendingForces;
    float mMass = 1.0f;
    Vector3 mGravity = Vector3(0.0f,0.0f,-980.0f);
    Vector3 mJumpForce = Vector3(0.0f,0.0f,35000.0f);
    float speed = 700.0f;
    float maxVelocity = 400.0f;
    float brakingFactor = 0.9f;
    Vector3 wallClimbForce = Vector3(0.0f, 0.0f, 1800.0f);
    float mWallClimbTimer = 0.0f;
    Vector3 mWallRunForce = Vector3(0.0f, 0.0f, 1200.0f);
    float mWallRunTimer = 0.0f;
    CollSide wallRunSide = CollSide::None;
    float WALL_CLIMB_LIMIT=0.4f;
    float CLIMB_SPEED_LIMIT=350.0f;
    class CheckPoint* nextCheckPoint;
    int mRunningSFX;
    float SOUND_RUN_LIMIT = 50.0f;
    float FALLING_LIMIT = -750.0f;
    float textTimer = 5.0f;
    
};
