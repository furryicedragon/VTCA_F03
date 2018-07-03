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
	setupOption();
	loadbar();

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
	titleSprite->setVisible(false);
	this->addChild(titleSprite, 1);


	auto startItem = MenuItemImage::create(GUI_startnomal, GUI_startclick, [&](Ref* sender)
	{
		this->buttonClickSound();
		MainGame::GetInstance()->setVisible(true);
		this->setVisible(false);
		MainGame::GetInstance()->gameStarto();
		experimental::AudioEngine::setVolume(bg_music_main, 0.0f);

	});

	//auto restartItem = MenuItemImage::create(GUI_start, GUI_start, [&](Ref* sender)
	//{
	//	//Tiếp tục chơi game. save
	//	//Code nhẩy vào chơi tiếp!
	//});

	auto optionItem = MenuItemImage::create(BT_optionnomal, BT_optionclick, [&](Ref* sender)
	{
		//Code nhẩy vào tùy chỉnh game!
		this->buttonClickSound();
		_menuGame->setVisible(false);
		titleSprite->setVisible(false);
		setupSeting->setVisible(true);
	});

	//auto masterItem = MenuItemImage::create(GUI_start, GUI_start, [&](Ref* sender)
	//{
	//	//Code nhẩy vào master!
	//	//Show tác giả!
	//});

	auto exitItem = MenuItemImage::create(BT_exitnomal, BT_exitclick, [&](Ref* sender)
	{
		//Code để thoát game!
		this->buttonClickSound();
		exitGame();
	});

	_menuGame = Menu::create(startItem, /*restartItem,*/ optionItem, /*masterItem,*/ exitItem, nullptr);
	_menuGame->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height * 0.35f));
	_menuGame->alignItemsVerticallyWithPadding(5.0f);
	_menuGame->setVisible(false);
	_background->addChild(_menuGame);
}

void MainMenuScene::setupOption()
{
	Size bgSize = _background->getContentSize();

	this->setupSeting = Layer::create();
	
	_bgOption = Sprite::create(GUI_backsetingpause);
	_bgOption->setScale(0.85f);
	_bgOption->setPosition(Vec2(bgSize.width / 2, bgSize.height / 2));

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

			this->sfxVolume = (float)percent / 100;
			break;
		}
	});
	sliderSFXvolume->setPercent(sfxVolume * 100);
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

			musicVolume = (float)percent / 100;
			experimental::AudioEngine::setVolume(bg_music_main, musicVolume);

			break;
		}
	});
	sliderMusicVolume->setPercent(experimental::AudioEngine::getVolume(bg_music_main) * 100);
	_bgOption->addChild(sliderMusicVolume);

	auto btBack = ui::Button::create(BT_back, BT_backclick);
	btBack->setScale(0.5f);
	btBack->setPosition(Vec2(_bgOptionSize.width * 0.2, _bgOptionSize.height * 0.1));
	btBack->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			this->buttonClickSound();
			break;

		case ui::Widget::TouchEventType::ENDED:
			setupSeting->setVisible(false);
			
			if (!MainScene::GetInstance()->gamePauseLayer->isVisible()) 
				_menuGame->setVisible(true);

			titleSprite->setVisible(true);
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
			this->buttonClickSound();
			break;

		case ui::Widget::TouchEventType::ENDED:
			setupSeting->setVisible(false);
			
			if (!MainScene::GetInstance()->gamePauseLayer->isVisible()) 
				_menuGame->setVisible(true);

			titleSprite->setVisible(true);
			break;
		}
	});
	_bgOption->addChild(btAccept);
	setupSeting->addChild(_bgOption);
	setupSeting->setVisible(false);
	_background->addChild(setupSeting, 19, 9981);
}

void MainMenuScene::buttonClickSound()
{
	experimental::AudioEngine::play2d("sounds/button_click.mp3", false, sfxVolume);
}

void MainMenuScene::bgAudio()
{
	bg_music_main = experimental::AudioEngine::play2d(RandomHelper::random_int(0, 1) ? BG_audio_1 : BG_audio_2 , true, musicVolume);
}

void MainMenuScene::update()
{

}

void MainMenuScene::loadbar()
{
	Size bgSize = _background->getContentSize();
	Sprite* loadBar = Sprite::create(UI_barfg);
	loadBar->setPosition(Vec2(bgSize.width / 2, bgSize.height / 2));
	loadBar->setTag(1);
	_background->addChild(loadBar);

	Sprite* barbg = Sprite::create(UI_barbg);
	barbg->setAnchorPoint(Vec2(bgSize.width / 2, bgSize.height / 2));
	barbg->setPosition(Vec2(bgSize.width / 2, bgSize.height / 2));

	mouseBar = ProgressTimer::create(barbg);
	mouseBar->setType(ProgressTimerType::BAR);
	mouseBar->setAnchorPoint(Vec2(0, 0));
	mouseBar->setPosition(Vec2(0, 0));
	mouseBar->setBarChangeRate(Vec2(1, 0));
	mouseBar->setMidpoint(Vec2(0, 0));
	mouseBar->setPercentage(0.0f);
	float timeline = mouseBar->getPercentage();

	auto action = Sequence::create(ProgressTo::create(1.0f, 100.0f), CallFunc::create([=]() { _menuGame->setVisible(true); titleSprite->setVisible(true); _background->removeChildByTag(1, true); }), nullptr);
	mouseBar->runAction(action);
	loadBar->addChild(mouseBar);

}

void MainMenuScene::exitGame()
{
	Director::getInstance()->end();
	
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
	#endif
}