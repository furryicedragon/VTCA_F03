#include "MainScene.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
	return MainScene::create();
}

bool MainScene::init()
{
	HUDLayer* hud = HUDLayer::create();
	this->addChild(hud, 10, 9999);

	MainGame* mainGame = MainGame::create();
	this->addChild(mainGame, 1, 8888);

	
	return true;
}
