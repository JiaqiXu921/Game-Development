#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
    if(mIsPaused==false&&!mAnimName.empty()){
        mAnimTimer+=mAnimFPS*deltaTime;
        //wrapping
        while(mAnimTimer >= mAnims.find(mAnimName)->second.size()){
            mAnimTimer -=mAnims.find(mAnimName)->second.size();
        }
    }
    this->SetTexture(mAnims.find(mAnimName)->second[(int)mAnimTimer]);
}

void AnimatedSprite::SetAnimation(const std::string& name, bool resetTimer)
{
	if (mAnimName != name)
	{
		mAnimName = name;
	}

	if (resetTimer)
	{
		mAnimTimer = 0.0f;
	}
}

void AnimatedSprite::AddAnimation(const std::string& name,
								  const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}
