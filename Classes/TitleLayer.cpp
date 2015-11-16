/******************************************************************************/
/*
@file	TitleLayer.cpp
@brief	TitleLayer 実装コードです。<br>

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/

#include "TitleLayer.h"
#include "GameLayer.h"
#include "GameManager.h"
#include "NendModule.h"
#include "NendInterstitialModule.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace CocosDenshion;

// -----------------------------------------------------------------------------
namespace
{
	enum {
		TAG_BUTTON_EASY = 5,
		TAG_BUTTON_NORMAL = 6,
		TAG_BUTTON_HARD = 7,
	};
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
TitleLayer::TitleLayer()
{
}

// -----------------------------------------------------------------------------
TitleLayer::~TitleLayer()
{
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
bool TitleLayer::init()
{
	// カラーレイヤを初期化をする
	if (!LayerColor::initWithColor(Color4B(240, 238, 229, 255))) {
		return false;
	}

	// レイアウトを作成する
	Button* uiButton = nullptr;
	auto layout = GUIReader::getInstance()->widgetFromJsonFile("UI_Title.json");
	auto bg = layout->getChildByName("BG");

	uiButton = dynamic_cast<Button*>(bg->getChildByName("Easy"));
	uiButton->addTouchEventListener(this, toucheventselector(TitleLayer::onPushCallback));
	uiButton = dynamic_cast<Button*>(bg->getChildByName("Normal"));
	uiButton->addTouchEventListener(this, toucheventselector(TitleLayer::onPushCallback));
	uiButton = dynamic_cast<Button*>(bg->getChildByName("Hard"));
	uiButton->addTouchEventListener(this, toucheventselector(TitleLayer::onPushCallback));
	this->addChild(layout, 1);
    
	// キーボードイベントを登録する
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(TitleLayer::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// Nend広告を表示する
    NendModule::createNADViewTop(NEND_API_KEY, NEND_SPOT_ID);
    NendInterstitialModule::createNADInterstitial(NEND_INTERSTITIAL_API_KEY, NEND_INTERSTITIAL_SPOT_ID);

	// BGMを鳴らす
	//if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
	//	SimpleAudioEngine::getInstance()->playBackgroundMusic("title.mp3", true);
	//}
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
void TitleLayer::gameExit()
{
	// 終了する
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	return;
#endif
	Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
void TitleLayer::onPushCallback(Ref* sender, TouchEventType type)
{
	CCLOG("push call back");
	if (type != TOUCH_EVENT_ENDED) {
		return;
	}
	Node* select = dynamic_cast<Node*>(sender);
	switch (select->getTag()) {
	case TAG_BUTTON_EASY:
		GameManager::setupGameLevel(GameManager::GAME_LEVEL_EASY);
		Director::getInstance()->replaceScene(TransitionFlipX::create(1.0f, GameScene::createScene()));
		break;
	case TAG_BUTTON_NORMAL:
		GameManager::setupGameLevel(GameManager::GAME_LEVEL_NORMAL);
		Director::getInstance()->replaceScene(TransitionFlipX::create(1.0f, GameScene::createScene()));
		break;
	case TAG_BUTTON_HARD:
		GameManager::setupGameLevel(GameManager::GAME_LEVEL_HARD);
		Director::getInstance()->replaceScene(TransitionFlipX::create(1.0f, GameScene::createScene()));
		break;
	default:
		break;
	}
}

// -----------------------------------------------------------------------------
void TitleLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_BACKSPACE:
		this->gameExit();
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
Scene* TitleScene::createScene()
{
	// シーンを作成する
	auto scene = Scene::create();
	auto sceneLayer = TitleLayer::create();

	// シーンにノードを追加する
	scene->addChild(sceneLayer, 1);
	return scene;
}

/******************************************************************************/
//	End Of File
/******************************************************************************/
