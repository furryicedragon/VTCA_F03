#pragma once
#ifndef MainGame_hpp
#define MainGame_hpp


#include<cocos2d.h>
#include"Player.h"
#include"Enemy.h"
#include"Skill.h"
#include"HUDLayer.h"
#include"StatPlayer.h"

USING_NS_CC;

class MainGame : public cocos2d::Node
{
public:
	CREATE_FUNC(MainGame);
	Size visibleSize;
	virtual bool init() override;
	Enemy* boss1m1;
	Enemy* boss2m1;
	Enemy* boss1m2;
	Enemy* boss2m2;
	Enemy* bossfm1;
	Enemy* bossfm2;
	Enemy* eee;
	Player* ppp;
	Sprite* nothingBar;
	ProgressTimer* HPonHead;
	ProgressTimer* HitDame;

	Sprite* pppPositionHelper;
	Sprite* dashHelper;
	Sprite* gameOver;
	Sprite* startGame;
	Sprite* congratulation;
	Sprite* finishPortal;

	ValueMap finishPoint;
	
	EventKeyboard::KeyCode lastKeyPressed;
	EventKeyboard::KeyCode anotherKeyPressed;

	std::vector<Rect> grounds;
	TMXLayer* meta;
	TMXTiledMap* map1;
	Vec2 lastMovePos; //for retrieving last y
	Point tileCoordForPosition(Point position);
	std::vector<EventKeyboard::KeyCode> repeatedKeys;
	std::vector<Enemy*> allEnemy;
	bool congratz;
	bool canRetry;
	bool boss1;
	bool boss2;
	bool onTouchBegan(Touch* touch, Event* event);
	bool keyPressed(EventKeyboard::KeyCode keyCode , Event* event);
	bool keyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	bool isRepeated;
	bool checkRange(Enemy* enemy2Check,int theRange);
	bool checkGravity();
	bool isGameOver;
	bool enemyAdded;
	bool isGameStart;
	bool canIdle;
	int currentWave;
	int currentMap;
	int currentBoss;
	int howManyKeyPressed;
	int count;
	void setupPressedKeyHandling();
	void setupTouchHandling();
	void whatYouWant(EventKeyboard::KeyCode keyCode,int yourStatus); //yourStatus = 1-idle,2-move,3-attack
	void updatePlayerPosition();
	void check4Directions(Point posDirection,int directionClock);
	void waveXMapXInit();
	void allEnemyInit();
	void spawnEffect(Enemy* enemy2Spawn,int index);
	void checkAttackRange(Enemy* eee,int index);
	void spawnPlayer();
	Animate* animation(std::string actionName, float timeEachFrame);
	void displayDamage(int damage, std::string color, Vec2 where, Size sizes);
	void delAll();

	Label* scoreLabel;
private:
	virtual void update(float elapsed) override;
	bool checkCollision(Player* pPlayer);

};
#endif // !MainGame_hpp