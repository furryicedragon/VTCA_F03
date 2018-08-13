#include "MainMenuScene.h"
#include "ftimage.h"
#include "ui\UISlider.h"
#include "ui\CocosGUI.h"
#include "ui\UIWidget.h"
#include "MainScene.h"
#include "MainGame.h"

USING_NS_CC;

using namespace std;

MainMenuScene * MainMenuScene::instance = NULL;

bool MainMenuScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	_background = Sprite::create(GUI_background);
	_background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(_background);

	bgAudio();
	setupMenuGame();
	//loadbar();

	MainMenuScene::instance = this;

	return true;
}

MainMenuScene * MainMenuScene::GetInstance() {
	return MainMenuScene::instance;
}

void MainMenuScene::setupMenuGame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	titleSprite = Sprite::create(GUI_title);
	titleSprite->setScale(1.5f);
	titleSprite->setPosition(visibleSize.width / 2, visibleSize.height * 0.8);
	this->addChild(titleSprite, 1);


	auto startItem = MenuItemImage::create(GUI_startnomal, GUI_startclick, [&](Ref* sender)
	{
		this->buttonClickSound();
		MainGame::GetInstance()->setVisible(true);
		this->setVisible(false);
		MainGame::GetInstance()->gameStarto();
		experimental::AudioEngine::setVolume(bg_music_main, 0.0f);

	});

	auto exitItem = MenuItemImage::create(BT_exitnomal, BT_exitclick, [&](Ref* sender)
	{
		//Code để thoát game!
		this->buttonClickSound();
		exitGame();
	});

	_menuGame = Menu::create(startItem, exitItem, nullptr);
	_menuGame->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height * 0.35f));
	_menuGame->alignItemsVerticallyWithPadding(5.0f);
	_background->addChild(_menuGame);
}

void MainMenuScene::buttonClickSound()
{
	experimental::AudioEngine::play2d("sounds/button_click.mp3", false, sfxVolume);
}

void MainMenuScene::bgAudio()
{
	bg_music_main = experimental::AudioEngine::play2d(RandomHelper::random_int(0, 1) ? BG_audio_1 : BG_audio_2 , true, musicVolume);
}

void MainMenuScene::exitGame()
{
	Director::getInstance()->end();
	
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
	#endif
}