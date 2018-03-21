#include "MainScene.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
	return MainScene::create();
}

bool MainScene::init()
{
	Node * mainGame = MainGame::create();
	this->addChild(mainGame,1);
	return true;
}
