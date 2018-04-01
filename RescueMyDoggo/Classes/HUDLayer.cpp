#include <HUDLayer.h>
USING_NS_CC;

bool HUDLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setupStick();
	this->setVisible(false);
	return true;
}

void HUDLayer::setupStick()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Rect joystickBaseDimensions;
	joystickBaseDimensions = Rect(0, 0, 160.0f, 160.0f);

	Point joystickBasePosition;
	joystickBasePosition = Vec2(visibleSize.width * 0.1f, visibleSize.height*0.1f);

	JoystickBase *joystickBase = new JoystickBase();
	joystickBase->init();
	joystickBase->setPosition(joystickBasePosition);
	joystickBase->setBackgroundSprite(Sprite::create("StickBase.png"));
	joystickBase->setThumbSprite(Sprite::create("Stick.png"));

	Joystick *aJoystick = new Joystick();
	aJoystick->initWithRect(joystickBaseDimensions);

	aJoystick->autorelease();
	joystickBase->setJoystick(aJoystick);
	joystickBase->setPosition(joystickBasePosition);

	movementStick = joystickBase->getJoystick();
	movementStick->retain();

	this->addChild(joystickBase);
}

void HUDLayer::toggleVisiblity()
{
	if (this->isVisible())
		this->setVisible(false);
	else
		this->setVisible(true);
}