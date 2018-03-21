#pragma once
#ifndef Player_hpp
#define Player_hpp

#include<cocos2d.h>
USING_NS_CC;
class Player :public cocos2d::Sprite {
public:
	static Player* create();
	std::vector<EventKeyboard::KeyCode> inputKeys;
	std::vector<int> attackFrameNumber;
	std::vector<bool> canMoveDirections;
	std::vector<bool> doneDamage;
	std::vector<Sprite*> effectStatus;
	std::vector<Label*> statUpBox;
	std::string weaponKind;
	std::string lastDirection;
	std::string secondLastDirection;
	Sprite* attackHelper;
	Sprite* slash;
	Sprite* movementHelper;
	Label* hp;
	Size map1Size;
	float lastDuration;
	float lastX;
	float lastY;
	float attackSpeed;
	float additionalAS;
	int count;
	int attackChainNumber;
	int presentDirectionInNumber;
	int previousDirectionInNumber;
	int checkDirectionInNumber(std::string direction);
	int damageCurrent;
	int additionalHP;
	int additionalDmg;
	bool isSpawning;
	bool isSpawn;
	bool isHit;
	bool isDead;
	bool isDashing;
	bool isHoldingKey;
	bool isMoving;
	bool isAttacking;
	bool notCombination;
	bool canDash;
	bool isRolling;
	bool canShowStatUp;
	void attack();
	void moving();
	void idleStatus();
	void attackCount();
	void smootherMove();
	void checkDash();
	void slashEffect();
	void getHit(int damage,float eeePosX);
	void roll();
	void setDoneDamageTo(bool whatYouWant);
	void setHP(int HP);
	void dead();
	Animate* animation(std::string actionName, float timeEachFrame);
	void forbidAllAction();
	void statUp(int damage,int HP,int speed);
	void spawnEffect();
private:
	void initOption();
};

#endif // !Player_hpp