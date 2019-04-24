#pragma once
#ifndef HUDLAYER_hpp
#define HUDLAYER_hpp

#include<cocos2d.h>
#include<JoystickBase.h>
#include<Button.h>
#include<ButtonBase.h>
#include<StatPlayer.h>
USING_NS_CC;

class HUDLayer : public Layer
{
public:
	CREATE_FUNC(HUDLayer);

	virtual bool init() override;
	 
	static HUDLayer * instance;
	static HUDLayer * GetInstance();
	void setupPause();

	Joystick *movementStick;
	void setupStick();

	Button *attackBtn;
	Button *rollBtn;
	Button *skill1Btn;
	Button *skill2Btn;
	void setupButtons();

	void resetHUDstate();

	void toggleVisiblity();
	void setupStat();
	StatPlayer* statPlayer;

	Label* scoreLabel;
	Sprite* scoreSprite;
	void DisplayScore();
};

#endif // !HUDLAYER_hpp
