#include <GameLayer.h>
#include <ui/CocosGUI.h>
#include <MainScene.h>
USING_NS_CC;

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->showMenuDead();
	this->setVisible(false);
	return true;
}
void GameLayer::showMenuDead()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	backOptionDead = Sprite::create(GUI_backMainmenu);
	backOptionDead->setScale(4);
	backOptionDead->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));
	if (backOptionDead)
	this->addChild(backOptionDead, 1900);
	//backOptionDead->setVisible(false);

	Size _bgOptionDead = backOptionDead->getContentSize();

	////show button ra menu hay chơi lại khi chết dưới cái back
	auto btBack = ui::Button::create(BT_HomeGame);



	//btBack->setPosition(backOptionDead->getPosition()/1.2);
	btBack->setPosition(Vec2(_bgOptionDead.width * 0.8, _bgOptionDead.height * 0.5));
	btBack->setScale(0.2);
	btBack->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			Scene *mainScene = MainScene::create();
			Director::getInstance()->pushScene(mainScene);

			break;
		}
	});

	auto btAccept = ui::Button::create(BT_RetryGame);

	btAccept->setPosition(Vec2(_bgOptionDead.width * 0.2, _bgOptionDead.height * 0.5));
	btAccept->setScale(0.2);
	btAccept->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			auto main_game = static_cast<MainGame*> (Director::getInstance()->getRunningScene()->getChildByTag(8888));
			main_game->retryGameDead();
			this->setVisible(false);
			break;

		}
	});
	backOptionDead->addChild(btBack);
	backOptionDead->addChild(btAccept);
}