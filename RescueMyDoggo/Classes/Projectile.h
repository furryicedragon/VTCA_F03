#pragma once
#ifndef Projectile_hpp
#define Projectile_hpp

#include"cocos2d.h"
USING_NS_CC;

class Projectile : public Sprite
{
public:
	static Projectile* create();

	void initOptions();

	std::vector<bool> canDamage;
	bool launching;

	void launch(Animate* anim, float range);
};

#endif // !Projectile_hpp
