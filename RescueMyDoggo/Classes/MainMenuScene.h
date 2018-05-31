#pragma once

#ifndef MainMenuScene_hpp
#define MainMenuScene_hpp

#include <cocos2d.h>
#include <ui\UISlider.h>
#include <ui\UIWidget.h>

USING_NS_CC;

#define GUI_background "UIUX/backgorund.png"
#define GUi_nameGame
#define GUI_backMainmenu "UIUX/bgMenu.png"
#define GUI_start "UIUX/shadedDark42.png"
#define GUI_restart "UIUX/shadedDark42.png"
#define GUI_option "UIUX/shadedDark42.png"
#define GUI_master "UIUX/shadedDark42.png"
#define GUI_exit "UIUX/shadedDark42.png"
#define SLI_bg "UIUX/Slider_Back.png"
#define SLI_pressbar "UIUX/Slider_PressBar.png"
#define SLI_disable "UIUX/SliderNode_Disable.png"
#define SLI_normal "UIUX/SliderNode_Normal.png"
#define SLI_press "UIUX/SliderNode_Press.png"
#define BT_back "UIUX/shadedDark21.png"
#define BT_accept "UIUX/shadedDark23.png"
#define UI_barfg "UIUX/LoadingBarFileRed.png"
#define UI_barbg "UIUX/LoadingBarFile.png"
#define BG_audio "UIUX/Polyphia-The_Worst.mp3"


class MainMenuScene : public cocos2d::Layer
{
public:

	virtual bool init() override;
	cocos2d::Sprite* _background, *_bgOption;
	void showBackGround();
	void setupMenuGame();
	void setupOption();

	Menu* _menuGame;

	float bgMusic = 0.3f, gameMusic = 0.3f;

	void bgAudio();

	void update();

	void loadbar();
	ProgressTimer* mouseBar;

	void exitGame();

	CREATE_FUNC(MainMenuScene);
};

#endif //MainMenuScene_hpp