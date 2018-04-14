#pragma once
#ifndef StatPlayer_
#define StatPlayer_

//#include"Player.h"
#include <cocos2d.h>
#include <string>
using namespace std;
USING_NS_CC;

class StatPlayer:public Layer
{
public:
	CREATE_FUNC(StatPlayer);
	virtual bool init() override;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	ProgressTimer* HPplayer;
	ProgressTimer* EXPplayer;
	ProgressTimer* DameHit;

	Sprite* avatar;
	Sprite* statBoard;
	Sprite* nothingBar;
	Sprite* nothingBar2;

	Label* level;

	void DisplayStat();
};

#endif // !StatPlayer_
