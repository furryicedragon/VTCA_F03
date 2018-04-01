#ifndef __SNEAKY_JOYSTICK_HPP
#define __SNEAKY_JOYSTICK_HPP

#include "cocos2d.h"

USING_NS_CC;

class Joystick : public Node
{
protected:
	float thumbRadiusSq;
	float deadRadiusSq;

	CC_SYNTHESIZE_READONLY(Point, stickPosition, StickPosition);
	CC_SYNTHESIZE_READONLY(float, degrees, Degrees);
	CC_SYNTHESIZE_READONLY(Point, velocity, Velocity);
	CC_SYNTHESIZE(bool, autoCenter, AutoCenter);
	CC_SYNTHESIZE_READONLY(bool, isDPad, IsDPad);
	CC_SYNTHESIZE(bool, hasDeadzone, HasDeadzone);
	CC_SYNTHESIZE(int, numberOfDirections, NumberOfDirections);
	CC_SYNTHESIZE(int, joystickRadiusSq, JoystickRadiusSq);


	CC_SYNTHESIZE_READONLY(float, joystickRadius, JoystickRadius);
	CC_SYNTHESIZE_READONLY(float, thumbRadius, ThumbRadius);
	CC_SYNTHESIZE_READONLY(float, deadRadius, DeadRadius);

	virtual ~Joystick();

	bool initWithRect(Rect rect);
	void setIsDPad(bool b);
	void setJoystickRadius(float r);
	void setThumbRadius(float r);
	void setDeadRadius(float r);
	static bool onTouchBegan(Touch *touch, Event *event);
	static void onTouchMoved(Touch *touch, Event *event);
	static void onTouchEnded(Touch *touch, Event *event);
	static void onTouchCancelled(Touch *touch, Event *event);

	void touchDelegateRelease();
	void touchDelegateRetain();

private:
	void updateVelocity(Point point);
	void setTouchRadius();

};
#endif
