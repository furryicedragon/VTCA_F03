#ifndef BUTTON_SKINNED_HPP
#define BUTTON_SKINNED_HPP

#include "Button.h"

class ButtonBase : public cocos2d::Layer
{
	CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, defaultSprite, DefaultSprite);
	CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, activatedSprite, ActivatedSprite);
	CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, disabledSprite, DisabledSprite);
	CC_SYNTHESIZE_READONLY(cocos2d::Sprite *, pressSprite, PressSprite);

	CC_SYNTHESIZE_READONLY(Button *, button, HUDbutton); //Not sure about this

	//Public methods
	CREATE_FUNC(ButtonBase);
	virtual ~ButtonBase();
	bool init();
	void watchSelf(float delta);
	void setContentSize(cocos2d::Size s);
	void setDefaultSprite(cocos2d::Sprite *aSprite);
	void setActivatedSprite(cocos2d::Sprite *aSprite);
	void setDisabledSprite(cocos2d::Sprite *aSprite);
	void setPressSprite(cocos2d::Sprite *aSprite);
	void setButton(Button *aButton);
};

#endif
