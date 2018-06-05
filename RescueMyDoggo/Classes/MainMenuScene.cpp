#include "MainMenuScene.h"
#include "ftimage.h"
#include "ui\UISlider.h"
#include "ui\CocosGUI.h"
#include "ui\UIWidget.h"
#include "SimpleAudioEngine.h"
#include "audio/include/AudioEngine.h"
#include "MainScene.h"
#include "MainGame.h"

USING_NS_CC;

using namespace std;
using namespace CocosDenshion;
using namespace experimental;

int bg_music_main;

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

	return true;
}

void MainMenuScene::setupMenuGame()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto startItem = MenuItemImage::create(GUI_startnomal, GUI_startclick, [&](Ref* sender)
	{
		/*thg thi ng ta de 
			auto mainScene chang han
			auto mainScene = static_cast< cai nay la kieu? cua Scene minh dinh lay, vd minh lay mainscene thi phai de vao MainScene*
			vd
			auto scenedattentdnchadc = static_cast<MainScene*> (Director::getInstance()->getRunningScene()->getChildByTag(8888));
																														vd 8888 la cua MainGame
																															vi sao?*/
		auto mainGameScene = static_cast<MainGame*> (Director::getInstance()->getRunningScene()->getChildByTag(8888));
		if (mainGameScene) 
		{ 
			mainGameScene->setVisible(true);
			this->setVisible(false);
			mainGameScene->gameStarto();
			experimental::AudioEngine::setVolume(bg_music_main, 0.0f);
		}
	});

	//auto restartItem = MenuItemImage::create(GUI_start, GUI_start, [&](Ref* sender)
	//{
	//	//Tiếp tục chơi game. save
	//	//Code nhẩy vào chơi tiếp!
	//});

	auto optionItem = MenuItemImage::create(BT_optionnomal, BT_optionclick, [&](Ref* sender)
	{
		//Code nhẩy vào tùy chỉnh game!
		_menuGame->setVisible(false);
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
	_bgOption->setScale(0.85);
	_bgOption->setPosition(Vec2(bgSize.width / 2, bgSize.height / 2));

	Size _bgOptionSize = _bgOption->getContentSize();

	auto sliderbgMusic = cocos2d::ui::Slider::create();
	sliderbgMusic->loadBarTexture(SLI_bg);
	sliderbgMusic->loadSlidBallTextures(SLI_normal, SLI_press, SLI_disable);
	sliderbgMusic->loadProgressBarTexture(SLI_pressbar);
	sliderbgMusic->setScale(1.5);
	sliderbgMusic->setPosition(Vec2(_bgOptionSize.width * 0.5, _bgOptionSize.height * 0.5));

	sliderbgMusic->addEventListener([&](Ref* sender, ui::Slider::EventType type) {
		switch (type)
		{
		case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
			ui::Slider* slider_receive = dynamic_cast<ui::Slider*>(sender);
			int bGPercent = slider_receive->getPercent();

			experimental::AudioEngine::setVolume(bg_music_main, (float)bGPercent / 100);

			break;
		}
	});
	sliderbgMusic->setPercent(experimental::AudioEngine::getVolume(bg_music_main) * 100);
	_bgOption->addChild(sliderbgMusic);

	auto slidergameMusic = cocos2d::ui::Slider::create();
	slidergameMusic->loadBarTexture(SLI_bg);
	slidergameMusic->loadSlidBallTextures(SLI_normal, SLI_press, SLI_disable);
	slidergameMusic->loadProgressBarTexture(SLI_pressbar);
	slidergameMusic->setScale(1.5);
	slidergameMusic->setPosition(Vec2(_bgOptionSize.width * 0.5, _bgOptionSize.height * 0.3));
	slidergameMusic->setPercent(gameMusic);

	slidergameMusic->addEventListener([&](Ref* sender, ui::Slider::EventType type) {
		switch (type)
		{
		case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:

			break;
		}
	});
	_bgOption->addChild(slidergameMusic);

	auto btBack = ui::Button::create(BT_back, BT_backclick);
	btBack->setScale(0.5f);
	btBack->setPosition(Vec2(_bgOptionSize.width * 0.2, _bgOptionSize.height * 0.1));
	btBack->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::ENDED:
			setupSeting->setVisible(false);
			auto gamepauseLayer = static_cast<Layer*> (Director::getInstance()->getRunningScene()->getChildByTag(9902));
			bool a = gamepauseLayer->isVisible();
			if(a == false) _menuGame->setVisible(true);
			else { }

			bgMusic = experimental::AudioEngine::getVolume(bg_music_main);
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
		case ui::Widget::TouchEventType::ENDED:
			setupSeting->setVisible(false);
			auto gamepauseLayer = static_cast<Layer*> (Director::getInstance()->getRunningScene()->getChildByTag(9902));
			bool a = gamepauseLayer->isVisible();
			if (a == false) _menuGame->setVisible(true);
			else { }

			bgMusic = experimental::AudioEngine::getVolume(bg_music_main);
			break;
		}
	});
	_bgOption->addChild(btAccept);
	setupSeting->addChild(_bgOption);
	setupSeting->setVisible(false);
	_background->addChild(setupSeting, 19, 9981);
}

void MainMenuScene::bgAudio()
{
	if (!bg_music_main)
		bg_music_main = experimental::AudioEngine::play2d(BG_audio, true, bgMusic);
	else
		experimental::AudioEngine::setVolume(bg_music_main, bgMusic);
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

	auto action = Sequence::create(ProgressTo::create(1.0f, 100.0f), CallFunc::create([=]() { _menuGame->setVisible(true); _background->removeChildByTag(1, true); }), nullptr);
	mouseBar->runAction(action);
	loadBar->addChild(mouseBar);

}

void MainMenuScene::exitGame()
{
	_background->removeAllChildren();

	Director::getInstance()->end();
	
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
	#endif
}