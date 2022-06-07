//
//  CheckPoint.hpp
//  Lab11
//
//  Created by Jiaqi Xu on 4/12/21.
//

#ifndef CheckPoint_hpp
#define CheckPoint_hpp

#include <stdio.h>
#include "Actor.h"
#include <string>
#endif /* CheckPoint_hpp */
class CheckPoint : public Actor {
public:
    CheckPoint(class Game* game,Actor* parent);
    void setIsActive(bool temp) {isActive = temp;}
    bool getIsActive() const {return isActive; }
    CollisionComponent* getCC() const {return cc;}
    virtual void OnUpdate(float deltaTime) override;
    void SetLevelString(std::string string) {mLevelString=string;}
    std::string GetLevelString() const {return mLevelString;}
    void SetCheckPointText(std::string string) {checkPointText=string;}
    std::string GetCheckPointText() const {return checkPointText;}
private:
    class MeshComponent* mc;
    class CollisionComponent* cc;
    bool isActive=false;
    std::string mLevelString;
    std::string checkPointText;
};
