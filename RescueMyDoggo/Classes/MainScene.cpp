#include "MainScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

MainScene * MainScene::instance = NULL;

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

	MainScene::instance = this;

	return true;
}

MainScene * MainScene::GetInstance() {
	return MainScene::instance;
}

void MainScene::setupMenuPause()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	this->gamePauseLayer = Layer::create();

	auto gamePause_bg = Sprite::create(GUI_menupause);
	gamePause_bg->setScale(0.8f);
	gamePause_bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	Size _bgOptionPause = gamePause_bg->getContentSize();

	// restart button
	auto btRestart = ui::Button::create(BT_restartnomal, BT_restartclick);

	btRestart->setPosition(Vec2(_bgOptionPause.width * 0.5f, _bgOptionPause.height * 0.52f));
	btRestart->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			MainMenuScene::GetInstance()->buttonClickSound();
			break;
		case ui::Widget::TouchEventType::ENDED:
			experimental::AudioEngine::stopAll();
			Director::getInstance()->resume();
			HUDLayer::GetInstance()->resetHUDstate();
			
			if (MainGame::GetInstance()->currentMap == 1) MainGame::GetInstance()->delAll();
			else MainGame::GetInstance()->delAll(MainGame::GetInstance()->currentMap);
			MainGame::GetInstance()->gameStarto();

			MainScene::GetInstance()->gamePauseLayer->setVisible(false);

			break;
		}
	});
	gamePause_bg->addChild(btRestart);

	auto btResume = ui::Button::create(BT_resumenomal, BT_resumeclick);

	btResume->setPosition(Vec2(_bgOptionPause.width * 0.5f, _bgOptionPause.height * 0.7f));
	btResume->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			MainMenuScene::GetInstance()->buttonClickSound();
			break;
		case ui::Widget::TouchEventType::ENDED:
			experimental::AudioEngine::resumeAll();
			Director::getInstance()->resume();
			
			HUDLayer::GetInstance()->setVisible(true);
			MainScene::GetInstance()->gamePauseLayer->setVisible(false);
			break;

		}
	});
	gamePause_bg->addChild(btResume);
	auto btExitGame = ui::Button::create(BT_exitnomal, BT_exitclick);

	btExitGame->setPosition(Vec2(_bgOptionPause.width * 0.5f, _bgOptionPause.height * 0.17f));
	btExitGame->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			MainMenuScene::GetInstance()->buttonClickSound();
			break;

		case ui::Widget::TouchEventType::ENDED:
			experimental::AudioEngine::stop(MainGame::GetInstance()->finalBossMusic);
			experimental::AudioEngine::stop(MainGame::GetInstance()->congratzMusic);
			experimental::AudioEngine::stop(MainGame::GetInstance()->bgm);
			MainGame::GetInstance()->delAll();
			MainGame::GetInstance()->setVisible(false);

			HUDLayer::GetInstance()->resetHUDstate();

			MainMenuScene::GetInstance()->bgAudio();
			MainMenuScene::GetInstance()->setVisible(true);

			MainScene::GetInstance()->gameOverLayer->setVisible(false);
			MainScene::GetInstance()->gamePauseLayer->setVisible(false);
			break;

		}
	});
	gamePause_bg->addChild(btExitGame);

	gamePauseLayer->addChild(gamePause_bg);
	gamePauseLayer->setVisible(false);
	this->addChild(gamePauseLayer, 12, 9902);
}

void MainScene::setupGameOverLayer()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	this->gameOverLayer = Layer::create();

	auto gameover_bg = Sprite::create(GUI_backDead);
	gameover_bg->setScale(0.8f);
	gameover_bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));


	Size _bgOptionDead = gameover_bg->getContentSize();

	auto highScoreTitle = Label::createWithSystemFont("Highscore:", "Arial", 40);
	highScoreTitle->setPosition(_bgOptionDead.width / 2, _bgOptionDead.height / 2 + 30);
	gameover_bg->addChild(highScoreTitle);

	auto highScoreLabel = Label::createWithSystemFont("0", "Arial", 50);
	highScoreLabel->setPositionX(highScoreTitle->getPositionX());
	highScoreLabel->setPositionY(highScoreTitle->getPositionY() - 55);
	highScoreLabel->setTag(10);
	gameover_bg->addChild(highScoreLabel);

	// main menu button
	auto btMenu = ui::Button::create(BT_homenomal, BT_homeclick);

	btMenu->setPosition(Vec2(_bgOptionDead.width * 0.7, _bgOptionDead.height * 0.2));
	btMenu->setScale(0.5f);
	btMenu->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			MainMenuScene::GetInstance()->buttonClickSound();
			break;
		case ui::Widget::TouchEventType::ENDED:
			experimental::AudioEngine::stop(MainGame::GetInstance()->finalBossMusic);
			experimental::AudioEngine::stop(MainGame::GetInstance()->congratzMusic);
			experimental::AudioEngine::stop(MainGame::GetInstance()->bgm);
			MainGame::GetInstance()->delAll();
			MainGame::GetInstance()->setVisible(false);

			HUDLayer::GetInstance()->resetHUDstate();
			
			MainMenuScene::GetInstance()->bgAudio();
			MainMenuScene::GetInstance()->setVisible(true);
			
			MainScene::GetInstance()->gameOverLayer->setVisible(false);
			break;
		}
	});
	gameover_bg->addChild(btMenu);

	// retry button
	auto btRetry = ui::Button::create(BT_restartnomal, BT_restartclick);

	btRetry->setPosition(Vec2(_bgOptionDead.width * 0.3, _bgOptionDead.height * 0.2));
	btRetry->setScale(0.5f);
	btRetry->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			MainMenuScene::GetInstance()->buttonClickSound();
			break;
		case ui::Widget::TouchEventType::ENDED:
			experimental::AudioEngine::stopAll();
			Director::getInstance()->resume();
			HUDLayer::GetInstance()->resetHUDstate();

			if (MainGame::GetInstance()->currentMap == 1) MainGame::GetInstance()->delAll();
			else MainGame::GetInstance()->delAll(MainGame::GetInstance()->currentMap);
			MainGame::GetInstance()->gameStarto();

			MainScene::GetInstance()->gameOverLayer->setVisible(false);

			break;
		}
	});
	gameover_bg->addChild(btRetry);

	gameOverLayer->addChild(gameover_bg, 0, 100);
	gameOverLayer->setVisible(false);
	this->addChild(gameOverLayer, 11, 9900);
}