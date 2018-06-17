#include <HUDLayer.h>
#include "ui\CocosGUI.h"
#include "MainMenuScene.h"
#include "MainScene.h"

USING_NS_CC;

HUDLayer * HUDLayer::instance = NULL;

bool HUDLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setupPause();
	this->setupStick();
	this->setupButtons();
	this->setupStat();
	this->DisplayScore();
	this->setVisible(false);

	HUDLayer::instance = this;
	return true;
}

HUDLayer * HUDLayer::GetInstance() {
	return HUDLayer::instance;
}

void HUDLayer::setupPause()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto btPause = ui::Button::create(BT_pauseNomal, BT_pauseClick);
	btPause->setPosition(Vec2(visibleSize.width * 0.95f, visibleSize.height * 0.93f));
	btPause->setScale(0.59f);
	btPause->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			this->setVisible(false);
			Director::getInstance()->pause();
			/*auto gamepauseLayer = static_cast<Layer*> (Director::getInstance()->getRunningScene()->getChildByTag(9902));
			gamepauseLayer->setVisible(true);*/
			MainScene::GetInstance()->gamePauseLayer->setVisible(true);
			break;
		}
	});
	this->addChild(btPause);
}

void HUDLayer::setupStick()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Rect joystickBaseDimensions;
	joystickBaseDimensions = Rect(0, 0, 160.0f, 160.0f);

	Point joystickBasePosition;
	joystickBasePosition = Vec2(visibleSize.width * 0.1f, visibleSize.height*0.125f);

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
	joystickBase->setScale(0.9f);

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
		buttonBase->setTag(1);

		attackBtn = buttonBase->getHUDbutton();
		attackBtn->retain();

		this->addChild(buttonBase);
	}
	
	// roll btn
	{
		buttonPosition = Vec2(visibleSize.width * 0.95f, visibleSize.height * 0.25f);

		ButtonBase *buttonBase = new ButtonBase();
		buttonBase->init();
		buttonBase->setPosition(buttonPosition);

		buttonBase->setDefaultSprite(Sprite::create("HUD/Skill_icon/roll.png"));
		buttonBase->setActivatedSprite(Sprite::create("HUD/Skill_icon/roll.png"));
		buttonBase->setDisabledSprite(Sprite::create("HUD/Skill_icon/roll.png"));
		buttonBase->setPressSprite(Sprite::create("HUD/Skill_icon/roll.png"));
		buttonBase->setOpacity(0.8);
		buttonBase->setScale(0.75f);

		Button *aButton = new Button();
		aButton->initWithRect(buttonDimensions);
		aButton->autorelease();

		buttonBase->setButton(aButton);
		buttonBase->setPosition(buttonPosition);
		buttonBase->setTag(2);

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
		buttonBase->setTag(3);

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
		buttonBase->setTag(4);

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

	//reset score label
	this->scoreLabel->setString("0");
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

	Vec2 StatPosition = Vec2(visibleSize.width* 0.25f, visibleSize.height * 0.92f);

	statPlayer = new StatPlayer();
	statPlayer->init();
	statPlayer->setPosition(StatPosition);
	statPlayer->autorelease();
	this->addChild(statPlayer);
}
void HUDLayer::DisplayScore()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 ScorePosition = Vec2(visibleSize.width * ((float)4/5), visibleSize.height * 0.93f);
	scoreSprite = Sprite::create("HUD/Bars/scoreBar.png");
	scoreSprite->setPosition(ScorePosition);
	scoreSprite->setScale(0.6f);

	scoreLabel = Label::createWithSystemFont("0", "Arial", 40.0f, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	scoreLabel->setColor(Color3B(255, 255, 255));
	scoreLabel->setPosition(scoreSprite->getPosition().x, scoreSprite->getPosition().y + 2);
	this->addChild(scoreSprite, 0);
	this->addChild(scoreLabel, 1);
}