//
//  HUD.hpp
//  Lab12
//
//  Created by Jiaqi Xu on 4/16/21.
//

#ifndef HUD_hpp
#define HUD_hpp

#include <stdio.h>
#include "UIComponent.h"
#endif /* HUD_hpp */
class HUD: public UIComponent{
public:
    HUD(class Actor* owner);
    ~HUD();
    virtual void Update(float deltaTime) override;
    virtual void Draw(class Shader* shader) override;
    void CoinCollected();
    void updateText();
    
private:
    class Font* mFont;
    class Texture* mTimerText;
    class Texture* mCoinText;
    class Texture* mRadarBack;
    class Texture* mArrow;
    class Texture* mBlip;
};
