/******************************************************************************/
/*
@file	ResultLayer.cpp
@brief	ResultLayer 実装コードです。<br>

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/

#include "ResultLayer.h"
#include "GameLayer.h"
#include "TitleLayer.h"
#include "GameManager.h"
#include "NendInterstitialModule.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

// -----------------------------------------------------------------------------
namespace
{
	enum {
		TAG_BUTTON_RETRY = 101,
		TAG_BUTTON_TITLE = 102,
		TAG_LABEL_RESULT = 201,
	};
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
ResultLayer::ResultLayer()
{
}

// -----------------------------------------------------------------------------
ResultLayer::~ResultLayer()
{
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
bool ResultLayer::init()
{
	// カラーレイヤを初期化をする
	if (!LayerColor::initWithColor(Color4B(240, 238, 229, 255))) {
		return false;
	}

	// レイアウトを作成する
	Button* uiButton = nullptr;
	Text* uiText = nullptr;
	auto layout = GUIReader::getInstance()->widgetFromJsonFile("ResultUI.json");
	uiButton = dynamic_cast<Button*>(layout->getChildByTag(TAG_BUTTON_RETRY));
	uiButton->addTouchEventListener(this, toucheventselector(ResultLayer::onPushCallback));
	uiButton = dynamic_cast<Button*>(layout->getChildByTag(TAG_BUTTON_TITLE));
	uiButton->addTouchEventListener(this, toucheventselector(ResultLayer::onPushCallback));
	uiText = dynamic_cast<Text*>(layout->getChildByTag(TAG_LABEL_RESULT));
	if (GameManager::isGameClear()) {
		uiText->setText("GAME CLEAR !!");
	} else if (GameManager::isGameOver()) {
		uiText->setText("GAME OVER");
	}
	this->addChild(layout, 1);
	this->scheduleOnce(schedule_selector(ResultLayer::showInterStatial), 1.2);
	return true;
}

void ResultLayer::showInterStatial(float dt)
{
    // インターステイシャル広告表示
    NendInterstitialModule::showNADInterstitialView();
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
void ResultLayer::onPushCallback(Ref* sender, TouchEventType type)
{
	if (type != TOUCH_EVENT_ENDED) {
		return;
	}
	Node* select = dynamic_cast<Node*>(sender);
	switch (select->getTag()) {
	case TAG_BUTTON_RETRY:
		GameManager::setupGameLevel(GameManager::getGameLevel());
		Director::getInstance()->replaceScene(TransitionFlipX::create(1.0f, GameScene::createScene()));
		break;
	case TAG_BUTTON_TITLE:
		Director::getInstance()->replaceScene(TransitionFlipX::create(1.0f, TitleScene::createScene()));
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
Scene* ResultScene::createScene()
{
	// シーンを作成する
	auto scene = Scene::create();
	auto sceneLayer = ResultLayer::create();

	// シーンにノードを追加する
	scene->addChild(sceneLayer, 1);
	return scene;
}

/******************************************************************************/
//	End Of File
/******************************************************************************/
