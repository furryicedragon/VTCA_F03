#include "MainScene.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
	return MainScene::create();
}

bool MainScene::init()
{
	MainMenuScene* menu = MainMenuScene::create();
	if (menu)
	this->addChild(menu, 2, 9090);

	HUDLayer* hud = HUDLayer::create();
	if(hud)
	this->addChild(hud, 10, 9999);

	MainGame* mainGame = MainGame::create();
	if (mainGame) {
		this->addChild(mainGame, 1, 8888);
		//mainGame->setVisible(false);
	}

	
	return true;
}
