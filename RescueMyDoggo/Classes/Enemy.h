#pragma once
#ifndef Enemy_hpp
#define Enemy_hpp

#include<cocos2d.h>
#include<Player.h>
USING_NS_CC;
class Enemy :public cocos2d::Sprite {
public:
	static Enemy* create(int xMapNumber, int xWaveNumber, int xBossNumber);
	Label* hp;
	Sprite* attackHelper;
	Sprite* movementHelper;
	Sprite* spell;
	Sprite* spellLanded;
	Size getHitFrameSize;
	Player* ppp;
	Animate* idleAnim;
	std::string combination;
	int skillDamage;
	int mapNumber;
	int waveNumber;
	int bossNumber;
	int useSkillLastFN;
	int skillLastFN;
	int mobilitySSAt;
	int moveSpeed;
	int visionRange;
	int norAtkDmgAfterF; // normalAttackDamageAfterFrames
	int doneAtkAfterF; //doneAttackAfterFrames;
	int inviTime;
	int getHitTime;
	float mobilitySpeed;
	float skillCD;
	float skillRange;
	float skillSpeed;
	float castSpeed;
	float pppFirstY;
	float line1X;
	float line2X;
	float line3X;
	float randomX;
	float randomY;
	float calculateDuration(float x, float y);
	bool invulnerable;
	bool breakTime;
	bool isSpawned;
	bool canDamage;
	bool canMove;
	bool canSpawn;
	bool isChasing;
	bool isSSMobility;
	bool isCaster;
	bool isOnCD;
	bool isIdle;
	bool isDead;
	bool isMoving;
	bool isAttacking;
	bool checkFrame(std::string action);
	void attack();
	void mobilitySS();
	void casterSpell();
	void randomMoving();
	void moving();
	void idleStatus();
	void getHit(int damage);
	void dead();
	void initOption();
	void setHP(int HP);
	void getFolderName();
	void getLastFrameNumberOf(std::string actionName);
	void attackLandedEffect();
	void forbidAllAction();
	void movingAnimation();
	void chasing();
	//void spawnEffect();
	Animate* animation(std::string actionName, float timeEachFrame);
private:
	virtual void update(float elapsed) override;
};

#endif // !Enemy_hpp