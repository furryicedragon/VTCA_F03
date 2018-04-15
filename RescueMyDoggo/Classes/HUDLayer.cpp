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
	this->setupStat();
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
		

		Label* cooldown = Label::createWithSystemFont("", "Calibri", 35.0f, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
		cooldown->setVisible(false);
		skill1Btn->addChild(cooldown, 1, 100);

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

		Label* cooldown = Label::createWithSystemFont("", "Calibri", 35.0f, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
		cooldown->setVisible(false);
		skill2Btn->addChild(cooldown, 1, 100);

		this->addChild(buttonBase);
	}
}

void HUDLayer::resetHUDstate()
{
	// reset cooldown labels
	auto cd1 = static_cast<Label*> (skill1Btn->getChildByTag(100));
	auto cd2 = static_cast<Label*> (skill2Btn->getChildByTag(100));

	cd1->setVisible(false);
	cd2->setVisible(false);

	// reset stat bar
	this->statPlayer->removeFromParentAndCleanup(true);
	this->setupStat();
}

void HUDLayer::toggleVisiblity()
{
	if (this->isVisible())
		this->setVisible(false);
	else
		this->setVisible(true);
}

void HUDLayer::setupStat()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Vec2 StatPosition = Vec2(visibleSize.width* 0.35f, visibleSize.height * 0.95);

	statPlayer = new StatPlayer();
	statPlayer->init();
	statPlayer->setPosition(StatPosition);
	statPlayer->autorelease();
	this->addChild(statPlayer);
}