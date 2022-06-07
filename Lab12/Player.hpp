//
//  Player.hpp
//  Lab07
//
//  Created by Jiaqi Xu on 3/15/21.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "Actor.h"
#endif /* Player_hpp */
class Player : public Actor {
public:
    Player(class Game* game, Actor* parent);
    class PlayerMove* getPM() const {return pm;}
    class CameraComponent* getCC() const {return cc;}
    class HUD* getHUD() const {return hud;}
    class CollisionComponent* getCollision() const {return collisionC;}
    void SetRespawnPos(Vector3 pos) {respawnPos = pos;}
    Vector3 GetRespawnPos() const {return respawnPos;}

private:
    Vector3 respawnPos;
    class Game* mGame;
    class PlayerMove* pm;
    class CameraComponent* cc;
    class CollisionComponent* collisionC;
    class HUD* hud;
};
