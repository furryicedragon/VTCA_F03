#pragma once
#ifndef MainScene_hpp
#define MainScene_hpp


#include<cocos2d.h>
#include"MainGame.h"
#include"HUDLayer.h"
#include"MainMenuScene.h"
#include "ui\CocosGUI.h"

USING_NS_CC;

class MainScene : public cocos2d::Scene
{
public:
	static Scene* createScene();
	CREATE_FUNC(MainScene);
	virtual bool init() override;

	static MainScene * instance;
	static MainScene * GetInstance();

	void setupMenuPause();
	Layer* gameOverLayer;
	Layer* gamePauseLayer;
	void setupGameOverLayer();
};
#endif // !MainScene_hpp