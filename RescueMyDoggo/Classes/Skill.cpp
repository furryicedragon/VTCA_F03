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

	for (int i = 1; i < 99; i++) {
		auto frameName = "/"+skillAName + "/(" + std::to_string(i) + ").png";
		Sprite* getSize = Sprite::create(frameName);
		if (!getSize)
			break;
		frames = i;
		Size theSize = getSize->getContentSize();
		auto frame = SpriteFrame::create(frameName, Rect(0, 0, theSize.width, theSize.height));
	}
	canDamage.resize(8);
	std::fill(canDamage.begin(), canDamage.end(), true);

	this->setVisible(false);
	this->launching = false;
	this->onCD = false;
}

void Skill::launch(Animate* anim, float range, float cooldown)
{
	launching = true;
	onCD = true;

	this->runAction(
		Sequence::create(
			Spawn::create(Repeat::create(anim, 2), MoveBy::create(0.5f, Vec2(range, 0)), nullptr),
			CallFunc::create([=]()
			{
				this->setVisible(false);
				this->launching = false;
				
				std::fill(canDamage.begin(), canDamage.end(), true);
			}), DelayTime::create(cooldown),
				CallFunc::create([=]()
				{
					onCD = false;
				}),
		nullptr));
}

void Skill::useSkill()
{
	this->setPosition(skillPosition);
}

void Skill::playerSkill()
{
}

