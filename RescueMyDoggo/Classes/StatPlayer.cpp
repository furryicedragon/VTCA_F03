#include "StatPlayer.h"
//#include "Player.h"

bool StatPlayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	this->DisplayStat();
	return true;
}
void StatPlayer::DisplayStat()
{
	statBoard = Sprite::create("HUD/Bars/statBoard.png");
	statBoard->setScale(0.5f);

	avatar = Sprite::create("HUD/Bars/avatar.png");
	avatar->setPosition(statBoard->getPosition().x - 255, statBoard->getPosition().y);
	avatar->setScale(0.6f);


	nothingBar = Sprite::create("HUD/Bars/nothing.png");
	nothingBar->setPosition(statBoard->getPosition().x + 50, statBoard->getPosition().y + 20);
	nothingBar->setScale(0.5f);

	nothingBar2 = Sprite::create("HUD/Bars/nothing.png");
	nothingBar2->setPosition(statBoard->getPosition().x + 50, statBoard->getPosition().y - 20);
	nothingBar2->setScale(0.5f);

	DameHit = ProgressTimer::create(Sprite::create("HUD/Bars/hitDame.png"));
	DameHit->setType(ProgressTimerType::BAR);
	DameHit->setPercentage(100);
	DameHit->setBarChangeRate(Vec2(1, 0));
	DameHit->setMidpoint(Vec2(0.0, 0.0));
	DameHit->setScale(0.5f);
	DameHit->setPosition(statBoard->getPosition().x + 50, statBoard->getPosition().y + 20);

	HPplayer = ProgressTimer::create(Sprite::create("HUD/Bars/hp.png"));
	HPplayer->setType(ProgressTimerType::BAR);
	HPplayer->setPercentage(100);
	HPplayer->setBarChangeRate(Vec2(1, 0));
	HPplayer->setMidpoint(Vec2(0.0, 0.0));
	HPplayer->setScale(0.5f);
	HPplayer->setPosition(statBoard->getPosition().x + 50, statBoard->getPosition().y + 20);


	EXPplayer = ProgressTimer::create(Sprite::create("HUD/Bars/exp.png"));
	EXPplayer->setType(ProgressTimerType::BAR);
	EXPplayer->setPercentage(0);
	EXPplayer->setBarChangeRate(Vec2(1, 0));
	EXPplayer->setMidpoint(Vec2(0.0, 0.0));
	EXPplayer->setScale(0.5f);
	EXPplayer->setPosition(statBoard->getPosition().x + 50, statBoard->getPosition().y - 20);

	level = Label::create();
	level->setString("1");
	level->setColor(Color3B( 255, 255, 255));
	level->setSystemFontSize(16);
	level->setPosition(statBoard->getPosition().x - 225, statBoard->getPosition().y - 30);

	this->addChild(statBoard, 1);
	this->addChild(avatar, 2);
	this->addChild(nothingBar, 2);
	this->addChild(nothingBar2, 2);
	this->addChild(DameHit, 3);
	this->addChild(HPplayer, 4);
	this->addChild(EXPplayer, 4);
	this->addChild(level, 4);

}