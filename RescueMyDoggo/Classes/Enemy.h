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
	Player* ppp;
	Animate* idleAnim;
	//std::string combination;
	SpriteFrameCache* eeeFrames;
	float skillDamage;
	int moneyRank;
	int mapNumber;
	int waveNumber;
	int bossNumber;
	int skillAtkAfterF; //useAttackSkillAfterFrames;
	int useSkillLastFN;
	int skillLastFN;
	int mobilitySSAt; //useMobilityAt
	int mobilityDoneAfterF; //doneAttackingUsingMobilityAfterFrame
	int moveSpeed;
	int visionRange;
	int norAtkDmgAfterF; // normalAttackDamageAfterFrames
	int doneAtkAfterF; //doneAttackAfterFrames;
	int inviTime;
	int getHitTime;
	int direction;
	float mobilitySpeed;
	float skillCD;
	float skillRange;
	float skillSpeed;
	float castSpeed;
	std::vector<float> listLineX;
	float randomX;
	float randomY;
	float spotPlayerLineLeft;
	float spotPlayerLineRight;

	bool canDrop;
	bool mobilityUsing;
	bool canRespawn;
	bool interuptable;
	bool invulnerable;
	bool breakTime;
	bool canChase;
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
	bool hpBar;
	void attack();
	void mobilitySS();
	void casterSpell();
	void randomMoving();
	void moving();
	void idleStatus();
	void getHit(int damage);
	void autoRespawn();
	void dead();
	void initOption();
	void setHP(int HP);
	//void getFolderName();
	void getLastFrameNumberOf(std::string actionName);
	void attackLandedEffect();
	void forbidAllAction();
	void movingAnimation();
	void chasing();
	//void spawnEffect();

	Map<std::string, Animate*> listAnimations;
	//Animate* animation(std::string actionName, float timeEachFrame);
	Animate * makeAnimation(std::string actionName, float timeEachFrame);
private:
	virtual void update(float elapsed) override;
};

#endif // !Enemy_hpp