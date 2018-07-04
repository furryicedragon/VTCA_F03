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
	setupPauseOption();
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

	////show button ra menu hay chơi lại khi chết dưới cái back
	auto btRestart = ui::Button::create(BT_restartnomal, BT_restartclick);

	btRestart->setPosition(Vec2(_bgOptionPause.width * 0.5f, _bgOptionPause.height * 0.52f));
	//btHome->setScale(0.9f);
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

	auto btOption = ui::Button::create(BT_optionnomal, BT_optionclick);

	btOption->setPosition(Vec2(_bgOptionPause.width * 0.5f, _bgOptionPause.height * 0.35f));
	//btOption->setScale(0.2f);
	btOption->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			MainMenuScene::GetInstance()->buttonClickSound();
			break;
		case ui::Widget::TouchEventType::ENDED:
			pauseSettingLayer->setVisible(true);
			gamePauseLayer->setVisible(false);
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
	//btExitGame->setScale(0.2f);
	btExitGame->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			MainMenuScene::GetInstance()->buttonClickSound();
			break;

		case ui::Widget::TouchEventType::ENDED:

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

void MainScene::setupPauseOption()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	this->pauseSettingLayer = Layer::create();

	auto _bgOption = Sprite::create(GUI_backsetingpause);
	_bgOption->setScale(0.85f);
	_bgOption->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	Size _bgOptionSize = _bgOption->getContentSize();

	auto SFXlabel = Label::createWithSystemFont("SFX", "Arial", 30.0f);
	SFXlabel->setPosition(Vec2(_bgOptionSize.width * 0.5, _bgOptionSize.height * 0.58));
	_bgOption->addChild(SFXlabel);

	auto sliderSFXvolume = cocos2d::ui::Slider::create();
	sliderSFXvolume->loadBarTexture(SLI_bg);
	sliderSFXvolume->loadSlidBallTextures(SLI_normal, SLI_press, SLI_disable);
	sliderSFXvolume->loadProgressBarTexture(SLI_pressbar);
	sliderSFXvolume->setScale(1.5);
	sliderSFXvolume->setPosition(Vec2(_bgOptionSize.width * 0.5, _bgOptionSize.height * 0.5));

	sliderSFXvolume->addEventListener([&](Ref* sender, ui::Slider::EventType type) {
		switch (type)
		{
		case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
			ui::Slider* slider_receive = dynamic_cast<ui::Slider*>(sender);
			int percent = slider_receive->getPercent();

			MainMenuScene::GetInstance()->sfxVolume = (float)percent / 100;
			break;
		}
	});
	sliderSFXvolume->setPercent(MainMenuScene::GetInstance()->sfxVolume * 100);
	_bgOption->addChild(sliderSFXvolume);

	auto musicLabel = Label::createWithSystemFont("Music", "Arial", 30.0f);
	musicLabel->setPosition(Vec2(_bgOptionSize.width * 0.5, _bgOptionSize.height * 0.38));
	_bgOption->addChild(musicLabel);

	auto sliderMusicVolume = cocos2d::ui::Slider::create();
	sliderMusicVolume->loadBarTexture(SLI_bg);
	sliderMusicVolume->loadSlidBallTextures(SLI_normal, SLI_press, SLI_disable);
	sliderMusicVolume->loadProgressBarTexture(SLI_pressbar);
	sliderMusicVolume->setScale(1.5);
	sliderMusicVolume->setPosition(Vec2(_bgOptionSize.width * 0.5, _bgOptionSize.height * 0.3));

	sliderMusicVolume->addEventListener([&](Ref* sender, ui::Slider::EventType type) {
		switch (type)
		{
		case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
			ui::Slider* slider_receive = dynamic_cast<ui::Slider*>(sender);
			int percent = slider_receive->getPercent();

			MainMenuScene::GetInstance()->musicVolume = (float)percent / 100;
			break;
		}
	});
	sliderMusicVolume->setPercent(experimental::AudioEngine::getVolume(MainMenuScene::GetInstance()->bg_music_main) * 100);
	_bgOption->addChild(sliderMusicVolume);

	auto btBack = ui::Button::create(BT_back, BT_backclick);
	btBack->setScale(0.5f);
	btBack->setPosition(Vec2(_bgOptionSize.width * 0.2, _bgOptionSize.height * 0.1));
	btBack->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			MainMenuScene::GetInstance()->buttonClickSound();
			break;

		case ui::Widget::TouchEventType::ENDED:
			pauseSettingLayer->setVisible(false);
			gamePauseLayer->setVisible(true);

			break;
		}
	});
	_bgOption->addChild(btBack);

	auto btAccept = ui::Button::create(BT_accept, BT_acceptclick);
	btAccept->setScale(0.5f);
	btAccept->setPosition(Vec2(_bgOptionSize.width * 0.8, _bgOptionSize.height * 0.1));
	btAccept->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{

		case ui::Widget::TouchEventType::BEGAN:
			MainMenuScene::GetInstance()->buttonClickSound();
			break;

		case ui::Widget::TouchEventType::ENDED:
			pauseSettingLayer->setVisible(false);
			gamePauseLayer->setVisible(true);

			break;
		}
	});
	_bgOption->addChild(btAccept);
	pauseSettingLayer->addChild(_bgOption);
	pauseSettingLayer->setVisible(false);

	this->addChild(pauseSettingLayer, 19);
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

	////show button ra menu hay chơi lại khi chết dưới cái back
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
			MainScene::GetInstance()->gamePauseLayer->setVisible(false);
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
		case ui::Widget::TouchEventType::BEGAN:
			MainMenuScene::GetInstance()->buttonClickSound();
			break;
		case ui::Widget::TouchEventType::ENDED:
			experimental::AudioEngine::stop(MainGame::GetInstance()->finalBossMusic);
			experimental::AudioEngine::stop(MainGame::GetInstance()->congratzMusic);
			experimental::AudioEngine::stop(MainGame::GetInstance()->bgm);
			MainScene::GetInstance()->gameOverLayer->setVisible(false);

			MainGame::GetInstance()->restartGame();
			break;

		}
	});
	gameover_bg->addChild(btRetry);

	gameOverLayer->addChild(gameover_bg, 0, 100);
	gameOverLayer->setVisible(false);
	this->addChild(gameOverLayer, 11, 9900);
}