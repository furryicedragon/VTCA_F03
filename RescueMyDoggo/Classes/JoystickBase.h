#ifndef __JOYSTICK_BASE_HPP
#define __JOYSTICK_BASE_HPP

#include "cocos2d.h"
#include "Joystick.h"

class JoystickBase : public cocos2d::Layer
{
public:
	CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, backgroundSprite, BackgroundSprite);
	CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, thumbSprite, ThumbSprite);
	CC_SYNTHESIZE_READONLY(Joystick *, joystick, Joystick);

	//Public methods
	CREATE_FUNC(JoystickBase);
	virtual ~JoystickBase();
	bool init();
	void updatePositions(float delta);
	void setContentSize(cocos2d::Size s);
	void setBackgroundSprite(cocos2d::Sprite *aSprite);
	void setThumbSprite(cocos2d::Sprite *aSprite);
	void setJoystick(Joystick *aJoystick);
};
#endif