#include "Skill.h"

Skill* Skill::create(float damage, float coolDown, float appearTime, float disappearTime,float MobilityTime, float MobilityDelayTime , Point skillPos, std::string skillName, std::string castName)
{
	Skill* pointerSprite = new Skill();
	if (pointerSprite && pointerSprite->initWithFile("/Enemies/Map 2/Wave 1/Spell/0.png"))
	{
		pointerSprite->autorelease();
		pointerSprite->coolDownTime = coolDown;
		pointerSprite->skillDamage = damage;
		pointerSprite->skillPosition = skillPos;
		pointerSprite->skillAppearTime = appearTime;
		pointerSprite->skillDisappearTime = disappearTime;
		pointerSprite->mobilityDelayTime = MobilityDelayTime;
		pointerSprite->mobilityTime = MobilityTime;
		pointerSprite->skillAName = skillName;
		pointerSprite->castAName = castName;
		pointerSprite->initOptions();

		return pointerSprite;
	}

	CC_SAFE_DELETE(pointerSprite);

	return nullptr;
}

void Skill::initOptions()
{
	Vector<SpriteFrame *> runningFrames;

	for (int i = 1; i < 99; i++) 
	{
		auto frameName = "/"+skillAName + "/(" + std::to_string(i) + ").png";
		Sprite* getSize = Sprite::create(frameName);
		if (!getSize)
			break;
		frames = i;
		Size theSize = getSize->getContentSize();
		auto frame = SpriteFrame::create(frameName, Rect(0, 0, theSize.width, theSize.height));
	}

	this->setVisible(false);
	this->launching = false;
	this->onCD = false;
}

void Skill::setupCD(Button * button)
{
	this->HUD_button = button;

	timer = this->coolDownTime;

	this->cd_displayer = static_cast<Label*> (this->HUD_button->getChildByTag(100));

	this->updateLabel();
	cd_displayer->setVisible(true);

	this->getScheduler()->schedule(schedule_selector(Skill::updateTimer), this, 0.1f, false);
}

void Skill::useSkill()
{
	this->setPosition(skillPosition);
}

void Skill::updateLabel()
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << timer;
	cd_displayer->setString(ss.str());
}

void Skill::updateTimer(float delta)
{
	timer -= delta;

	updateLabel();

	if (timer <= 0)
	{
		this->getScheduler()->unschedule(schedule_selector(Skill::updateTimer), this);
		cd_displayer->setVisible(false);
		this->onCD = false;
	}

}

void Skill::playerSkill()
{
}

