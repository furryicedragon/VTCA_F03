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


bool MainMenuScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	showBackGround();
	loadbar();

	bgAudio();

	return true;
}

void MainMenuScene::showBackGround()
{
	Size winSize = Director::getInstance()->getWinSize();
	_background = Sprite::create(GUI_background);
	_background->setPosition(Point(winSize.width / 2, winSize.height / 2));
	this->addChild(_background);
}

void MainMenuScene::showMenuGame()
{
	Size bgSize = _background->getContentSize();

	auto startItem = MenuItemImage::create(GUI_start, GUI_start, [&](Ref* sender)
	{
		/*thg thi ng ta de 
			auto mainScene chang han
			auto mainScene = static_cast< cai nay la kieu? cua Scene minh dinh lay, vd minh lay mainscene thi phai de vao MainScene*
			vd
			auto scenedattentdnchadc = static_cast<MainScene*> (Director::getInstance()->getRunningScene()->getChildByTag(8888));
																														vd 8888 la cua MainGame
																															vi sao?*/
		auto mainGameScene = static_cast<MainGame*> (Director::getInstance()->getRunningScene()->getChildByTag(8888));
		if (mainGameScene == nullptr) { }
		mainGameScene->setVisible(true);
		this->setLocalZOrder(0);
		mainGameScene->gameStarto();
	});
	startItem->setPosition(Point(bgSize.width / 2, bgSize.height / 2 * 1.2));

	auto restartItem = MenuItemImage::create(GUI_start, GUI_start, [&](Ref* sender)
	{
		//Tiếp tục chơi game. save
		//Code nhẩy vào chơi tiếp!
	});
	restartItem->setPosition(Point(bgSize.width / 2, bgSize.height / 2 * 1.05));

	auto optionItem = MenuItemImage::create(GUI_start, GUI_start, [&](Ref* sender)
	{
		//Code nhẩy vào tùy chỉnh game!
		_background->removeChild(_menuGame, true);
		showOption();
	});
	optionItem->setPosition(Point(bgSize.width / 2, bgSize.height / 2 * 0.9));

	auto masterItem = MenuItemImage::create(GUI_start, GUI_start, [&](Ref* sender)
	{
		//Code nhẩy vào master!
		//Show tác giả!
	});
	masterItem->setPosition(Point(bgSize.width / 2, bgSize.height / 2 * 0.75));

	auto exitItem = MenuItemImage::create(GUI_start, GUI_start, [&](Ref* sender)
	{
		//Code để thoát game!
		exitGame();
	});
	exitItem->setPosition(Point(bgSize.width / 2, bgSize.height / 2 * 0.6));

	_menuGame = Menu::create(startItem, restartItem, optionItem, masterItem, exitItem, nullptr);
	_menuGame->setPosition(Point(Size::ZERO));
	_menuGame->setVisible(true);
	_background->addChild(_menuGame);
}

void MainMenuScene::showOption()
{
	Size bgSize = _background->getContentSize();
	Sprite* _bgOption = Sprite::create(GUI_backMainmenu);
	_bgOption->setScale(3);
	_bgOption->setPosition(Point(bgSize.width / 2, bgSize.height / 2));

	Size _bgOptionSize = _bgOption->getContentSize();

	auto sliderbgMusic = cocos2d::ui::Slider::create();
	sliderbgMusic->loadBarTexture(SLI_bg);
	sliderbgMusic->loadSlidBallTextures(SLI_normal, SLI_press, SLI_disable);
	sliderbgMusic->loadProgressBarTexture(SLI_pressbar);
	sliderbgMusic->setScale(0.5);
	sliderbgMusic->setPosition(Point(_bgOptionSize.width / 2, _bgOptionSize.height / 2 * 1.5));
	sliderbgMusic->setPercent(bgMusic);

	sliderbgMusic->addEventListener([&](Ref* sender, ui::Slider::EventType type) {
		switch (type)
		{
		case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
			ui::Slider* slider_receive = dynamic_cast<ui::Slider*>(sender);
			int bGPercent = sliderbgMusic->getPercent();

			// set volume here. via AudioEngine::setVolume();
			//Utility::getInstance()->setMusicVolume((float)percent / 100);
			//experimental::AudioEngine::setVolume(bgMusic, (float)(percent + 1) / 100);
			/*float _bGPercent = (bGPercent + 1) / 100;
			if (experimental::AudioEngine::getVolume(bgMusic) != 0.0f)
			{
				experimental::AudioEngine::setVolume(bgMusic, (float)_(bGPercent + 1) / 100);
			}*/
			break;
		}
	});

	auto slidergameMusic = cocos2d::ui::Slider::create();
	slidergameMusic->loadBarTexture(SLI_bg);
	slidergameMusic->loadSlidBallTextures(SLI_normal, SLI_press, SLI_disable);
	slidergameMusic->loadProgressBarTexture(SLI_pressbar);
	slidergameMusic->setScale(0.5);
	slidergameMusic->setPosition(Point(_bgOptionSize.width / 2, _bgOptionSize.height / 2 * 0.5));
	slidergameMusic->setPercent(gameMusic);

	slidergameMusic->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			
			break;
		default:
			break;
		}
	});
	
	

	_bgOption->addChild(sliderbgMusic);
	_bgOption->addChild(slidergameMusic);

	auto btBack = ui::Button::create(BT_back);
	
	btBack->setPosition(Point(bgSize.width / 2 * 0.8, bgSize.height / 2 * 0.6));
	btBack->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			_background->removeAllChildren();
			//showMenuGame();
			break;
		default:
			break;
		}
	});
	_background->addChild(btBack);
	auto btAccept = ui::Button::create(BT_accept);

	btAccept->setPosition(Point(bgSize.width / 2 * 1.2, bgSize.height / 2 * 0.6));
	btAccept->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			_background->removeAllChildren();
			showMenuGame();
			break;
		default:
			break;
		}
	});
	_background->addChild(btAccept);
	
	_background->addChild(_bgOption);
}

void MainMenuScene::showSliderOption()
{
	Size bgSize = _background->getContentSize();	
}

void MainMenuScene::bgAudio()
{
	auto bgAudio = AudioEngine::play2d(BG_audio);
	AudioEngine::setLoop(bgAudio, true);
	AudioEngine::setVolume(bgAudio, bgMusic);
}

void MainMenuScene::update()
{

}

void MainMenuScene::loadbar()
{
	Size bgSize = _background->getContentSize();
	Sprite* border = Sprite::create(UI_barfg);
	border->setPosition(Point(bgSize.width / 2, bgSize.height / 2));
	_background->addChild(border);

	Sprite* barbg = Sprite::create(UI_barbg);
	barbg->setAnchorPoint(Point(bgSize.width / 2, bgSize.height / 2));
	barbg->setPosition(Point(bgSize.width / 2, bgSize.height / 2));

	mouseBar = ProgressTimer::create(barbg);
	mouseBar->setType(ProgressTimerType::BAR);
	mouseBar->setAnchorPoint(Vec2(0, 0));
	mouseBar->setPosition(Vec2(0, 0));
	mouseBar->setBarChangeRate(Vec2(1, 0));
	mouseBar->setMidpoint(Vec2(0, 0));
	mouseBar->setPercentage(0.0f);
	float timeline = mouseBar->getPercentage();

	auto action = Sequence::create(ProgressTo::create(3.0f, 100.0f), CallFunc::create([=]() {_background->removeAllChildren(), this->showMenuGame(); }), nullptr);
	mouseBar->runAction(action);
	border->addChild(mouseBar);

}

void MainMenuScene::exitGame()
{
	_background->removeAllChildren();

	Director::getInstance()->end();
	
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
	#endif
}