#pragma once
#ifndef Player_hpp
#define Player_hpp

#include<cocos2d.h>
#include<Projectile.h>
USING_NS_CC;
class Player :public cocos2d::Sprite {
public:
	static Player* create();
	std::vector<EventKeyboard::KeyCode> inputKeys;
	std::vector<int> attackFrameNumber;
	std::vector<bool> canMoveDirections;
	std::vector<bool> doneDamage;
	std::vector<Sprite*> effectStatus;
	std::string weaponKind;
	std::string lastDirection;
	std::string secondLastDirection;
	Sprite* attackHelper;
	Sprite* slash;
	Sprite* movementHelper;
	Label* hp;
	Label* statPlus;
	Label* level;
	Size map1Size;
	float lastDuration;
	float lastX;
	float lastY;
	float attackSpeed;
	int currentEXP;
	int baseEXP;
	int baseHP;
	int w1kills;
	int w2kills;
	int count;
	int attackChainNumber;
	int presentDirectionInNumber;
	int previousDirectionInNumber;
	int checkDirectionInNumber(std::string direction);
	int damageCurrent;
	bool isSpawning;
	bool isSpawn;
	bool isHit;
	bool isDead;
	bool isHoldingKey;
	bool isMoving;
	bool isAttacking;
	bool isRolling;
	bool canShowStatUp;
	void attack();
	void moving();
	void idleStatus();
	void attackCount();
	void smootherMove();
	void slashEffect();
	void getHit(int damage,float eeePosX);
	void roll();
	void setDoneDamageTo(bool whatYouWant);
	void setHP(int HP);
	void dead();
	Animate* animation(std::string actionName, float timeEachFrame);
	void forbidAllAction();
	void statUp();
	void spawnEffect();
	void levelUp();

	Projectile* skill1;
	float cd_reduction;
	void launchSkill1();
	
private:
	void initOption();
	virtual void update(float elapsed) override;
};

#endif // !Player_hpp