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
	}
	setupGameOverLayer();

	return true;
}

void MainScene::setupGameOverLayer()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto gameOverLayer = Layer::create();

	auto gameover_bg = Sprite::create(GUI_backMainmenu);
	gameover_bg->setScale(4);
	gameover_bg->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));


	Size _bgOptionDead = gameover_bg->getContentSize();

	////show button ra menu hay chơi lại khi chết dưới cái back
	auto btMenu = ui::Button::create(BT_HomeGame);


	btMenu->setPosition(Vec2(_bgOptionDead.width * 0.8, _bgOptionDead.height * 0.5));
	btMenu->setScale(0.2f);
	btMenu->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			auto mainGame = static_cast<MainGame*>(this->getChildByTag(8888));
			mainGame->delAll();
			mainGame->setVisible(false);

			auto hud_layer = static_cast<HUDLayer*> (this->getChildByTag(9999));
			hud_layer->resetHUDstate();
			
			auto menu = static_cast<MainMenuScene*>(this->getChildByTag(9090));
			menu->bgAudio();
			menu->setVisible(true);
			
			auto layer = static_cast<Layer*>(this->getChildByTag(9900));
			layer->setVisible(false);
			break;
		}
	});
	gameover_bg->addChild(btMenu);

	auto btRetry = ui::Button::create(BT_RetryGame);

	btRetry->setPosition(Vec2(_bgOptionDead.width * 0.2, _bgOptionDead.height * 0.5));
	btRetry->setScale(0.2f);
	btRetry->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			auto layer = static_cast<Layer*>(this->getChildByTag(9900));
			layer->setVisible(false);

			auto mainGame = static_cast<MainGame*>(this->getChildByTag(8888));  
			mainGame->restartGame();
			break;

		}
	});
	gameover_bg->addChild(btRetry);

	gameOverLayer->addChild(gameover_bg);
	gameOverLayer->setVisible(false);
	this->addChild(gameOverLayer, 11, 9900);
}