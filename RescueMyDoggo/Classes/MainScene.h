#pragma once
#ifndef MainScene_hpp
#define MainScene_hpp


#include<cocos2d.h>
#include<MainGame.h>
#include<HUDLayer.h>
USING_NS_CC;

class MainScene : public cocos2d::Scene
{
public:
	static Scene* createScene();
	CREATE_FUNC(MainScene);
	virtual bool init() override;
};
#endif // !MainScene_hpp