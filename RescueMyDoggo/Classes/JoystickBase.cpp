#include "JoystickBase.h"

using namespace cocos2d;

JoystickBase::~JoystickBase()
{
	if (backgroundSprite)
	{
		backgroundSprite->release();
		backgroundSprite = NULL;
	}
	if (thumbSprite)
	{
		thumbSprite->release();
		thumbSprite = NULL;
	}

	if (joystick)
	{
		joystick->release();
		joystick = NULL;
	}
}

bool JoystickBase::init()
{
	bool pRet = false;
	if (Layer::init())
	{
		this->backgroundSprite = NULL;

		this->thumbSprite = NULL;

		this->joystick = NULL;

		this->schedule(schedule_selector(JoystickBase::updatePositions));
		pRet = true;
	}
	return pRet;
}

void JoystickBase::updatePositions(float delta)
{
	if (joystick && thumbSprite)
	{
		thumbSprite->setPosition(joystick->getStickPosition());
	}
}

void JoystickBase::setContentSize(Size s)
{
	Layer::setContentSize(s);
	backgroundSprite->setContentSize(s);
	//joystick.joystickRadius = s.width/2;
}

void JoystickBase::setBackgroundSprite(Sprite *aSprite)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	if (backgroundSprite)
	{
		if (backgroundSprite->getParent())
		{
			backgroundSprite->getParent()->removeChild(backgroundSprite, true);
		}
		backgroundSprite->release();
	}
#endif

	backgroundSprite = aSprite;
	if (aSprite)
	{
		this->addChild(backgroundSprite, 0);
		this->setContentSize(backgroundSprite->getContentSize());
	}
}

void JoystickBase::setThumbSprite(Sprite *aSprite)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	if (thumbSprite)
	{
		if (thumbSprite->getParent())
		{
			thumbSprite->getParent()->removeChild(thumbSprite, true);
		}
		thumbSprite->release();
	}
#endif

	thumbSprite = aSprite;
	if (aSprite)
	{
		this->addChild(thumbSprite, 1);
		//joystick->setThumbRadius(thumbSprite->getContentSize().width/2);
	}
}

void JoystickBase::setJoystick(Joystick *aJoystick)
{
	if (joystick)
	{
		if (joystick->getParent())
		{
			joystick->getParent()->removeChild(joystick, true);
		}
		joystick->release();
	}
	aJoystick->retain();
	joystick = aJoystick;
	if (aJoystick)
	{
		this->addChild(aJoystick, 2);
		if (thumbSprite)
		{
			joystick->setThumbRadius(thumbSprite->getBoundingBox().size.width / 2);
		}
		else
		{
			joystick->setThumbRadius(0);
		}
		if (backgroundSprite)
		{
			joystick->setJoystickRadius(backgroundSprite->getBoundingBox().size.width / 2);
		}
	}
}