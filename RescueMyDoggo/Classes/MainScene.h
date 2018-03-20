#pragma once

#ifndef MainScene_hpp
#define MainScene_hpp

#include <cocos2d.h>

USING_NS_CC;

class MainScene : public cocos2d::Scene
{
public:
	static Scene* createScene();

	virtual bool init() override;

	CREATE_FUNC(MainScene);
};

#endif //MainScene_hpp