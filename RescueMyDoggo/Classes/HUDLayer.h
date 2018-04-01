#pragma once
#ifndef HUDLAYER_hpp
#define HUDLAYER_hpp

#include<cocos2d.h>
#include<JoystickBase.h>
USING_NS_CC;

class HUDLayer : public Layer
{
public:
	CREATE_FUNC(HUDLayer);

	virtual bool init() override;

	Joystick *movementStick;
	void setupStick();

	void toggleVisiblity();
};

#endif // !HUDLAYER_hpp
