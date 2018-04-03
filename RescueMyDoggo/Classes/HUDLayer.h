#pragma once
#ifndef HUDLAYER_hpp
#define HUDLAYER_hpp

#include<cocos2d.h>
#include<JoystickBase.h>
#include<Button.h>
#include<ButtonBase.h>
USING_NS_CC;

class HUDLayer : public Layer
{
public:
	CREATE_FUNC(HUDLayer);

	virtual bool init() override;

	Joystick *movementStick;
	void setupStick();

	Button *attackBtn;
	Button *rollBtn;
	Button *skill1Btn;
	Button *skill2Btn;
	void setupButtons();

	void displayCD(Button* button, float cooldown);

	void toggleVisiblity();
};

#endif // !HUDLAYER_hpp
