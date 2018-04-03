#pragma once
#ifndef Skill_hpp
#define Skill_hpp

#include"cocos2d.h"
#include <string>
#include <iomanip>
#include "Button.h"
#include "ButtonBase.h"
USING_NS_CC;

class Skill : public Sprite
{
public:
	static Skill* create(float damage, float coolDown, float appearTime,float disappearTime , float MobilityTime,float MobilityDelayTime, Point skillPos, std::string skillName, std::string castActionName);

	void initOptions();

	Point skillPosition;
	float frames;
	float skillDisappearTime;
	float mobilityTime;
	float mobilityDelayTime;
	float skillAppearTime;
	float skillDamage;
	float coolDownTime;
	std::vector<bool> canDamage;
	bool launching;
	bool onCD;
	std::string skillAName;
	std::string castAName;


	Button* HUD_button;
	Label* cd_displayer;
	float timer;

	void setupCD(Button* button);

	void useSkill();
	void playerSkill();

	void updateLabel();

	void updateTimer(float delta);
};

#endif // !Skill_hpp
