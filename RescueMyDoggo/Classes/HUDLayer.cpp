#include <HUDLayer.h>
USING_NS_CC;

bool HUDLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setupStick();
	this->setupButtons();
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
	joystickBase->setBackgroundSprite(Sprite::create("HUD/Joystick/StickBase.png"));
	joystickBase->setThumbSprite(Sprite::create("HUD/Joystick/Stick.png"));

	Joystick *aJoystick = new Joystick();
	aJoystick->initWithRect(joystickBaseDimensions);

	aJoystick->autorelease();
	joystickBase->setJoystick(aJoystick);
	joystickBase->setPosition(joystickBasePosition);

	movementStick = joystickBase->getJoystick();
	movementStick->retain();

	this->addChild(joystickBase);
}

void HUDLayer::setupButtons()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	Rect buttonDimensions = Rect(0, 0, 64.0f, 64.0f);
	Point buttonPosition;

	// attack btn
	{
		buttonPosition = Vec2(visibleSize.width * 0.9f, visibleSize.height * 0.1f);

		ButtonBase *buttonBase = new ButtonBase();
		buttonBase->init();
		buttonBase->setPosition(buttonPosition);

		buttonBase->setDefaultSprite(Sprite::create("HUD/Skill_icon/attack_normal.png"));
		buttonBase->setActivatedSprite(Sprite::create("HUD/Skill_icon/attack_normal.png"));
		buttonBase->setDisabledSprite(Sprite::create("HUD/Skill_icon/attack_normal.png"));
		buttonBase->setPressSprite(Sprite::create("HUD/Skill_icon/attack_normal.png"));
		buttonBase->setOpacity(0.8);

		Button *aButton = new Button();
		aButton->initWithRect(buttonDimensions);
		aButton->autorelease();

		buttonBase->setButton(aButton);
		buttonBase->setPosition(buttonPosition);

		attackBtn = buttonBase->getHUDbutton();
		attackBtn->retain();
		this->addChild(buttonBase);
	}
	
	// roll btn
	{
		buttonPosition = Vec2(visibleSize.width * 0.9f, visibleSize.height * 0.25f);

		ButtonBase *buttonBase = new ButtonBase();
		buttonBase->init();
		buttonBase->setPosition(buttonPosition);

		buttonBase->setDefaultSprite(Sprite::create("HUD/Skill_icon/roll.png"));
		buttonBase->setActivatedSprite(Sprite::create("HUD/Skill_icon/roll.png"));
		buttonBase->setDisabledSprite(Sprite::create("HUD/Skill_icon/roll.png"));
		buttonBase->setPressSprite(Sprite::create("HUD/Skill_icon/roll.png"));
		buttonBase->setOpacity(0.8);

		Button *aButton = new Button();
		aButton->initWithRect(buttonDimensions);
		aButton->autorelease();

		buttonBase->setButton(aButton);
		buttonBase->setPosition(buttonPosition);

		rollBtn = buttonBase->getHUDbutton();
		rollBtn->retain();
		this->addChild(buttonBase);
	}

	// skill 1 btn
	{
		buttonPosition = Vec2(visibleSize.width * 0.775f, visibleSize.height * 0.1f);

		ButtonBase *buttonBase = new ButtonBase();
		buttonBase->init();
		buttonBase->setPosition(buttonPosition);

		buttonBase->setDefaultSprite(Sprite::create("HUD/Skill_icon/launch_projectile.png"));
		buttonBase->setActivatedSprite(Sprite::create("HUD/Skill_icon/launch_projectile.png"));
		buttonBase->setDisabledSprite(Sprite::create("HUD/Skill_icon/launch_projectile.png"));
		buttonBase->setPressSprite(Sprite::create("HUD/Skill_icon/launch_projectile.png"));
		buttonBase->setOpacity(0.8);

		Button *aButton = new Button();
		aButton->initWithRect(buttonDimensions);
		aButton->autorelease();

		buttonBase->setButton(aButton);
		buttonBase->setPosition(buttonPosition);

		skill1Btn = buttonBase->getHUDbutton();
		skill1Btn->retain();
		this->addChild(buttonBase);

	}

	// skill 2 btn
	{
		buttonPosition = Vec2(visibleSize.width * 0.65f, visibleSize.height * 0.1f);

		ButtonBase *buttonBase = new ButtonBase();
		buttonBase->init();
		buttonBase->setPosition(buttonPosition);

		buttonBase->setDefaultSprite(Sprite::create("HUD/Skill_icon/dash_stab.png"));
		buttonBase->setActivatedSprite(Sprite::create("HUD/Skill_icon/dash_stab.png"));
		buttonBase->setDisabledSprite(Sprite::create("HUD/Skill_icon/dash_stab.png"));
		buttonBase->setPressSprite(Sprite::create("HUD/Skill_icon/dash_stab.png"));
		buttonBase->setOpacity(0.8);

		Button *aButton = new Button();
		aButton->initWithRect(buttonDimensions);
		aButton->autorelease();

		buttonBase->setButton(aButton);
		buttonBase->setPosition(buttonPosition);

		skill2Btn = buttonBase->getHUDbutton();
		skill2Btn->retain();
		this->addChild(buttonBase);
	}
}

void HUDLayer::displayCD(Button* button, float cooldown)
{
	auto mainGameLayer = static_cast<MainGame*> (Director::getInstance()->getRunningScene()->getChildByTag(8888));
}

void HUDLayer::toggleVisiblity()
{
	if (this->isVisible())
		this->setVisible(false);
	else
		this->setVisible(true);
}