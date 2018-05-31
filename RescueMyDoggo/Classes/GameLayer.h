#pragma once
#ifndef GAMELAYER_hpp
#define GAMELAYER_hpp

#include<cocos2d.h>
#include<JoystickBase.h>
#include<Button.h>
#include<ButtonBase.h>
#include<MainGame.h>
#include<StatPlayer.h>
USING_NS_CC;

class GameLayer : public Layer
{
public:
	CREATE_FUNC(GameLayer);

	virtual bool init() override;

	void showMenuDead();
	Sprite* backOptionDead;
};

#endif // !GameLayer_hpp
