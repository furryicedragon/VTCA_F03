#include "MainScene.h"
#include "MainMenuScene.h"

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

	setupMenuPause();
	setupGameOverLayer();

	return true;
}

void MainScene::setupMenuPause()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	this->gamePauseLayer = Layer::create();

	auto gamePause_bg = Sprite::create(GUI_menupause);
	gamePause_bg->setScale(0.8f);
	gamePause_bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	Size _bgOptionPause = gamePause_bg->getContentSize();

	////show button ra menu hay chơi lại khi chết dưới cái back
	auto btHome = ui::Button::create(BT_restartnomal, BT_restartclick);

	btHome->setPosition(Vec2(_bgOptionPause.width * 0.5f, _bgOptionPause.height * 0.52f));
	//btHome->setScale(0.9f);
	btHome->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->resume();
			auto hud_layer = static_cast<HUDLayer*> (this->getChildByTag(9999));
			hud_layer->resetHUDstate();

			auto mainGame = static_cast<MainGame*>(this->getChildByTag(8888));
			mainGame->delAll(mainGame->currentMap);
			mainGame->gameStarto();

			auto pause_layer = static_cast<Layer*> (this->getChildByTag(9902));
			pause_layer->setVisible(false);

			break;
		}
	});
	gamePause_bg->addChild(btHome);

	auto btOption = ui::Button::create(BT_optionnomal, BT_optionclick);

	btOption->setPosition(Vec2(_bgOptionPause.width * 0.5f, _bgOptionPause.height * 0.35f));
	//btOption->setScale(0.2f);
	btOption->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			
			break;
		}
	});
	gamePause_bg->addChild(btOption);

	auto btResume = ui::Button::create(BT_resumenomal, BT_resumeclick);

	btResume->setPosition(Vec2(_bgOptionPause.width * 0.5f, _bgOptionPause.height * 0.7f));
	//btResume->setScale(0.2f);
	btResume->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->resume();
			auto hud_layer = static_cast<HUDLayer*> (this->getChildByTag(9999));
			hud_layer->setVisible(true);

			auto pause_layer = static_cast<Layer*> (this->getChildByTag(9902));
			pause_layer->setVisible(false);
			break;

		}
	});
	gamePause_bg->addChild(btResume);
	auto btExitGame = ui::Button::create(BT_exitnomal, BT_exitclick);

	btExitGame->setPosition(Vec2(_bgOptionPause.width * 0.5f, _bgOptionPause.height * 0.17f));
	//btExitGame->setScale(0.2f);
	btExitGame->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			this->removeAllChildren();

			Director::getInstance()->end();

			#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				exit(0);
			#endif
			break;

		}
	});
	gamePause_bg->addChild(btExitGame);

	gamePauseLayer->addChild(gamePause_bg);
	gamePauseLayer->setVisible(false);
	this->addChild(gamePauseLayer, 12, 9902);
}

void MainScene::setingPause()
{
	
}

void MainScene::setupGameOverLayer()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto gameOverLayer = Layer::create();

	auto gameover_bg = Sprite::create(GUI_backDead);
	gameover_bg->setScale(0.8f);
	gameover_bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));


	Size _bgOptionDead = gameover_bg->getContentSize();

	////show button ra menu hay chơi lại khi chết dưới cái back
	auto btMenu = ui::Button::create(BT_homenomal, BT_homeclick);


	btMenu->setPosition(Vec2(_bgOptionDead.width * 0.7, _bgOptionDead.height * 0.2));
	btMenu->setScale(0.5f);
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

			auto layerPause = static_cast<Layer*>(this->getChildByTag(9902));
			layerPause->setVisible(false);
			break;
		}
	});
	gameover_bg->addChild(btMenu);

	auto btRetry = ui::Button::create(BT_restartnomal, BT_restartclick);

	btRetry->setPosition(Vec2(_bgOptionDead.width * 0.3, _bgOptionDead.height * 0.2));
	btRetry->setScale(0.5f);
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