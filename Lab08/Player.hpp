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
    Player(class Game* game);
    class PlayerMove* getPM() {return pm;}
    class PlayerUI* getUI() {return ui;}
private:
    
    class Game* mGame;
    class MeshComponent* mc;
    class PlayerMove* pm;
    class CameraComponent* cc;
    class PlayerUI* ui;
};
