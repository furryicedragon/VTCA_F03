#pragma once
#ifndef Skill_hpp
#define Skill_hpp

#include"cocos2d.h"
USING_NS_CC;

class Skill : public Sprite
{
public:
	static Skill* create();

	void initOptions();

	std::vector<bool> canDamage;
	bool launching;
	bool onCD;

	void launch(Animate* anim, float range, float cooldown);
};

#endif // !Skill_hpp
