#pragma once

#ifndef MainMenuScene_hpp
#define MainMenuScene_hpp

#include <cocos2d.h>
#include <ui\UISlider.h>
#include <ui\UIWidget.h>
#include "AudioEngine.h"

USING_NS_CC;

#define GUI_background "UIUX/bgdfblur.png"
#define GUI_title "UIUX/title.png"
#define GUI_backMainmenu "UIUX/bgMenu.png"
#define GUI_menupause "UIUX/optionMenuinGame.png"
#define GUI_backsetingpause "UIUX/optionbg.png"
#define GUI_backDead "UIUX/backDead.png"

#define SLI_bg "UIUX/Slider_Back.png"
#define SLI_pressbar "UIUX/Slider_PressBar.png"
#define SLI_disable "UIUX/SliderNode_Disable.png"
#define SLI_normal "UIUX/SliderNode_Normal.png"
#define SLI_press "UIUX/SliderNode_Press.png"

#define UI_barfg "UIUX/LoadingBarFileRed.png"
#define UI_barbg "UIUX/LoadingBarFile.png"
#define BG_audio_1 "sounds/Polyphia-The_Worst.mp3"
#define BG_audio_2 "sounds/Polyphia-Crosty.mp3"

#define BT_HomeGame "UIUX/button_start.png"
#define BT_RetryGame "UIUX/retry_button.png"
#define BT_pauseNomal "UIUX/pauseHUDLayer.png"
#define BT_pauseClick "UIUX/pauseHUDLayer1.png"

#define BT_resumenomal "UIUX/resumnomal.png"
#define BT_resumeclick "UIUX/resumeclick.png"
#define BT_restartnomal "UIUX/restartnomal.png"
#define BT_restartclick "UIUX/restartclick.png"
#define BT_optionnomal "UIUX/setingnomal.png"
#define BT_optionclick "UIUX/setingclick.png"
#define BT_exitnomal "UIUX/exitnomal.png"
#define BT_exitclick "UIUX/exitclick.png"
#define BT_back "UIUX/X.png"
#define BT_backclick "UIUX/Xclick.png"
#define BT_accept "UIUX/V.png"
#define BT_acceptclick "UIUX/Vclick.png"
#define BT_homenomal "UIUX/homenomal.png"
#define BT_homeclick "UIUX/homeclick.png"
#define GUI_startnomal "UIUX/buttonStartnomal.png"
#define GUI_startclick "UIUX/buttonStartclick.png"


class MainMenuScene : public cocos2d::Layer
{
public:

	virtual bool init() override;
	cocos2d::Sprite* _background, *titleSprite;
	void setupMenuGame();

	Menu* _menuGame;

	static MainMenuScene * instance;
	static MainMenuScene * GetInstance();

	float musicVolume = 0.3f, sfxVolume = 1.0f;

	void buttonClickSound();
	void bgAudio();

	void loadbar();
	int bg_music_main;

	void exitGame();

	CREATE_FUNC(MainMenuScene);
};

#endif //MainMenuScene_hpp