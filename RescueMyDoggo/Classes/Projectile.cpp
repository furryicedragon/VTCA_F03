#include "Projectile.h"

Projectile* Projectile::create()
{
	Projectile* pointerSprite = new Projectile();
	if (pointerSprite && pointerSprite->initWithFile("/Enemies/Map 2/Wave 1/Spell/0.png"))
	{
		pointerSprite->autorelease();
		pointerSprite->initOptions();

		return pointerSprite;
	}

	CC_SAFE_DELETE(pointerSprite);

	return nullptr;
}

void Projectile::initOptions()
{
	canDamage.resize(8);
	std::fill(canDamage.begin(), canDamage.end(), true);

	this->setVisible(false);
	this->launching = false;
}

void Projectile::launch(Animate* anim, float range)
{
	launching = true;

	this->runAction(
		Sequence::create(
			Spawn::create(Repeat::create(anim, 2), MoveBy::create(0.5f, Vec2(range, 0)), nullptr),
			CallFunc::create([=]()
	{
		this->setVisible(false);
		this->launching = false;

		std::fill(canDamage.begin(), canDamage.end(), true);
	}), nullptr));
}
