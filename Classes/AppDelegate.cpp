/******************************************************************************/
/*
@file	AppDelegate.cpp
@brief	AppDelegate 実装コードです。<br>

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/

#include "AppDelegate.h"
#include "TitleLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
AppDelegate::AppDelegate()
{
}

// -----------------------------------------------------------------------------
AppDelegate::~AppDelegate() 
{
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
bool AppDelegate::applicationDidFinishLaunching()
{
    // GLViewが作成されていない場合は作成する
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (glview == nullptr) {
		glview = GLView::createWithRect("My Game",Rect(0, 0, 640, 960) );
        director->setOpenGLView(glview);
    }
	// デザイン解像度を(640x960)に設定する
	glview->setDesignResolutionSize(640.0f, 960.0f, ResolutionPolicy::SHOW_ALL);

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	setResourceRootPath("../Resources");
#endif

    // ディスプレイステータスを表示する
#if COCOS2D_DEBUG
    director->setDisplayStats(true);
#endif

	// FPSを設定する(1/60秒)
    director->setAnimationInterval(1.0 / 60);

    // シーンを実行する
    auto scene = TitleScene::createScene();
    director->runWithScene(scene);
    return true;
}

// -----------------------------------------------------------------------------
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// -----------------------------------------------------------------------------
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

/******************************************************************************/
//	End Of File
/******************************************************************************/
