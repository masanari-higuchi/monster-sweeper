/******************************************************************************/
/*
@file	GameLayer.cpp
@brief	GameLayer 実装コードです。<br>

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/

#include "GameLayer.h"
#include "TitleLayer.h"
#include "ResultLayer.h"
#include "GameManager.h"
#include "AppResUtil.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

// -----------------------------------------------------------------------------
namespace
{
	enum {
		TAG_TEXT_PLAYER_HP = 201,
		TAG_TEXT_PLAYER_LEVEL = 56,
		TAG_TEXT_PLAYER_EXP = 203,
		TAG_TEXT_PLAYER_NEXT = 55,
	};
	enum {
		TAG_LAYOUT_ENEMY_LEVEL = 1,
		TAG_LAYOUT_ENEMY_ICON = 2,
		TAG_LAYOUT_ENEMY_NUM = 3,
		TAG_IMAGE_HART     = 200,
		TAG_TEXT_ENEMY_LEVEL = 1201,
		TAG_IMAGE_ENEMY_ICON = 2301,
		TAG_TEXT_ENEMY_NUM = 3201,
	};
	enum {
		TAG_BUTTON_BOTTOM_CLOSE = 39,
	};
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
GameLayer::GameLayer()
	:	_seq(SEQ_NUM)
{
}

// -----------------------------------------------------------------------------
GameLayer::~GameLayer()
{
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
bool GameLayer::init()
{
	// カラーレイヤを初期化をする
	if (!LayerColor::initWithColor(Color4B(240, 238, 229, 255))) {
		return false;
	}

	// メインを読み込み
	auto layout = dynamic_cast<Widget*>(GUIReader::getInstance()->widgetFromJsonFile("Ui_GameMain.json"));
	//auto layout = dynamic_cast<Widget*>(root->getChildByName("Panel_53"));

	this->initPlayerLayout(layout);
	this->initEnemyLayout(layout);

	auto uiButton = dynamic_cast<Button*>(layout->getChildByTag(TAG_BUTTON_BOTTOM_CLOSE));
	CCLOG("close %p", uiButton);
	uiButton->addTouchEventListener(this, toucheventselector(GameLayer::onPushCallback));
	this->addChild(layout, 1);

	// レイアウトを作成する
	//this->createPlayerLayout();
	//this->createEnemyLayout();
	//this->createBottomLayout();

	// 更新イベントを登録する
	this->scheduleUpdate();
	return true;
}

// -----------------------------------------------------------------------------
void GameLayer::update(float dt)
{
	switch (_seq.getNo()) {
	case SEQ_RUN:
		seqRun();
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
void GameLayer::seqRun()
{
	// レイアウトを更新する
	this->applyPlayerLayout();
	this->applyEnemyLayout();
}

////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
void GameLayer::initPlayerLayout(Widget* layout)
{
	_playerUiParam._lvText = dynamic_cast<Text*>(layout->getChildByTag(TAG_TEXT_PLAYER_LEVEL));
	CCLOG("lv_text:%p", _playerUiParam._lvText);
	_playerUiParam._nextText = dynamic_cast<Text*>(layout->getChildByTag(TAG_TEXT_PLAYER_LEVEL)->getChildByTag(TAG_TEXT_PLAYER_NEXT));
	CCLOG("next:%p", _playerUiParam._nextText);
	_playerUiParam._nextbar = dynamic_cast<LoadingBar*>(layout->getChildByTag(TAG_TEXT_PLAYER_LEVEL)->getChildByTag(87));
	CCLOG("bar:%p", _playerUiParam._nextbar);

	auto life = dynamic_cast<Text*>(layout->getChildByName("Life"));
	for (int i = 0; i < LIFE_MAX; i++) {
		_playerUiParam._lifeImage[i] = dynamic_cast<ImageView*>(life->getChildByName(AppResUtil::getLifeImage(i+1)));
	}
}
void GameLayer::createPlayerLayout()
{
	// プレイヤーレイアウトを作成する
	auto layout = GUIReader::getInstance()->widgetFromJsonFile("GamePlayerUI.json");
	_playerUiParam._hpText = dynamic_cast<Text*>(layout->getChildByTag(TAG_TEXT_PLAYER_HP));
	_playerUiParam._lvText = dynamic_cast<Text*>(layout->getChildByTag(TAG_TEXT_PLAYER_LEVEL));
	_playerUiParam._expText = dynamic_cast<Text*>(layout->getChildByTag(TAG_TEXT_PLAYER_EXP));
	_playerUiParam._nextText = dynamic_cast<Text*>(layout->getChildByTag(TAG_TEXT_PLAYER_NEXT));
	this->addChild(layout, 1);
}

// -----------------------------------------------------------------------------
void GameLayer::initEnemyLayout(Widget* layout)
{
	float MARGIN = 7.0f;
	auto root = dynamic_cast<ImageView*>(layout->getChildByName("MonsterBase"));

	for (int i = 0; i < ENEMY_NUM; ++i) {
		auto monster = root->getChildByName(AppResUtil::getMosterImage(i+1));
		_enemyUiParam[i]._image   = dynamic_cast<ImageView*>(monster);
		_enemyUiParam[i]._lvText  = dynamic_cast<Text*>(monster->getChildByName("level"));
		_enemyUiParam[i]._numText = dynamic_cast<Text*>(monster->getChildByName("count"));
//		auto layoutParam = LinearLayoutParameter::create();
//		layoutParam->setMargin(Margin(MARGIN, 0, MARGIN, 0));
//		_enemyUiParam[i]._lvText->setLayoutParameter(layoutParam);
//		_enemyUiParam[i]._numText->setLayoutParameter(layoutParam);
//		_enemyUiParam[i]._image->setLayoutParameter(layoutParam);
	}

	const std::vector<GameManager::EnemyParam>& enemys = GameManager::getEnemyParam();
	float onesSize = _enemyUiParam[0]._image->getSize().width + MARGIN * 2.0f;
    float sumWidth = onesSize * enemys.size();

	// 使用するUIを選出する
	for (auto enemy : enemys) {
		int index = enemy._level - 1;
		_enemyUiParam[index]._isUse = true;
	}
	// 使用していないUIをレイアウトから削除する
	for (int i = 0; i < ENEMY_NUM; ++i) {
		do {
			CC_BREAK_IF(_enemyUiParam[i]._isUse);
			root->removeChild(_enemyUiParam[i]._image);
			_enemyUiParam[i]._lvText = nullptr;
			_enemyUiParam[i]._image = nullptr;
			_enemyUiParam[i]._numText = nullptr;
		} while(0);
	}
    //　水平方法にレイアウトを調整する
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
    float leftWidth = (visibleSize.width * 0.5f) + origin.x - (sumWidth * 0.5f);
    for (int i = 0; i < ENEMY_NUM; ++i) {
		do {
			CC_BREAK_IF(!_enemyUiParam[i]._isUse);
            auto& image = _enemyUiParam[i]._image;
            image->setPosition(Point(leftWidth, image->getPosition().y));
            leftWidth += onesSize;
		} while(0);
    }
}

void GameLayer::createEnemyLayout()
{
	// 敵レイアウトを作成する
	float MARGIN = 7.0f;
	auto rootLayout = GUIReader::getInstance()->widgetFromJsonFile("GameEnemyUI.json");
	auto levelLayout = dynamic_cast<Layout*>(rootLayout->getChildByTag(TAG_LAYOUT_ENEMY_LEVEL));
	auto iconLayout = dynamic_cast<Layout*>(rootLayout->getChildByTag(TAG_LAYOUT_ENEMY_ICON));
	auto numLayout = dynamic_cast<Layout*>(rootLayout->getChildByTag(TAG_LAYOUT_ENEMY_NUM));

	// UIを取得,自動調整用マージンを設定する
	for (int i = 0; i < ENEMY_NUM; ++i) {
		_enemyUiParam[i]._lvText = dynamic_cast<Text*>(levelLayout->getChildByTag(TAG_TEXT_ENEMY_LEVEL + i));
		_enemyUiParam[i]._image = dynamic_cast<ImageView*>(iconLayout->getChildByTag(TAG_IMAGE_ENEMY_ICON + i));
		_enemyUiParam[i]._numText = dynamic_cast<Text*>(numLayout->getChildByTag(TAG_TEXT_ENEMY_NUM + i));
		auto layoutParam = LinearLayoutParameter::create();
		layoutParam->setMargin(Margin(MARGIN, 0, MARGIN, 0));
		_enemyUiParam[i]._lvText->setLayoutParameter(layoutParam);
		_enemyUiParam[i]._numText->setLayoutParameter(layoutParam);
		_enemyUiParam[i]._image->setLayoutParameter(layoutParam);
	}
	const std::vector<GameManager::EnemyParam>& enemys = GameManager::getEnemyParam();
	float sumWidth = _enemyUiParam[0]._lvText->getSize().width + MARGIN * 2.0f;
	sumWidth *= enemys.size();

	// 使用するUIを選出する
	for (auto enemy : enemys) {
		int index = enemy._level - 1;
		_enemyUiParam[index]._isUse = true;
	}
	// 使用していないUIをレイアウトから削除する
	for (int i = 0; i < ENEMY_NUM; ++i) {
		do {
			CC_BREAK_IF(_enemyUiParam[i]._isUse);
			levelLayout->removeChild(_enemyUiParam[i]._lvText);
			iconLayout->removeChild(_enemyUiParam[i]._image);
			numLayout->removeChild(_enemyUiParam[i]._numText);
			_enemyUiParam[i]._lvText = nullptr;
			_enemyUiParam[i]._image = nullptr;
			_enemyUiParam[i]._numText = nullptr;
		} while(0);
	}
	// レイアウトサイズを調整,水平方向に自動調整する
	levelLayout->setSize(Size(sumWidth, levelLayout->getSize().height));
	iconLayout->setSize(Size(sumWidth, iconLayout->getSize().height));
	numLayout->setSize(Size(sumWidth, numLayout->getSize().height));
	levelLayout->setLayoutType(LAYOUT_LINEAR_HORIZONTAL);
	iconLayout->setLayoutType(LAYOUT_LINEAR_HORIZONTAL);
	numLayout->setLayoutType(LAYOUT_LINEAR_HORIZONTAL);

	// レイアウト位置を調整する
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();
	float centerWidth = visibleSize.width / 2 + origin.x;
	float labelLayoutDelta = levelLayout->convertToNodeSpace(Point(centerWidth, 0)).x - levelLayout->getSize().width * 0.5f;
	float iconLayoutDelta = iconLayout->convertToNodeSpace(Point(centerWidth, 0)).x - iconLayout->getSize().width * 0.5f;
	float numLayoutDelta = numLayout->convertToNodeSpace(Point(centerWidth, 0)).x - numLayout->getSize().width * 0.5f;
	levelLayout->setPosition(Point(levelLayout->getPosition().x + labelLayoutDelta, levelLayout->getPosition().y));
	iconLayout->setPosition(Point(iconLayout->getPosition().x + iconLayoutDelta, iconLayout->getPosition().y));
	numLayout->setPosition(Point(numLayout->getPosition().x + numLayoutDelta, numLayout->getPosition().y));
	this->addChild(rootLayout, 1);
}

// -----------------------------------------------------------------------------
void GameLayer::createBottomLayout()
{
	// 下部レイアウトを作成する
	Button* uiButton = nullptr;
	auto layout = GUIReader::getInstance()->widgetFromJsonFile("GameBottomUI.json");
	uiButton = dynamic_cast<Button*>(layout->getChildByTag(TAG_BUTTON_BOTTOM_CLOSE));
	uiButton->addTouchEventListener(this, toucheventselector(GameLayer::onPushCallback));
	this->addChild(layout, 1);
}

// -----------------------------------------------------------------------------
void GameLayer::applyPlayerLayout()
{
	// プレイヤーレイアウトを更新する
	const GameManager::PlayerParam& playerParam = GameManager::getPlayerParam();
	std::stringstream stream;
	//stream << "HP : " << playerParam._hp;
	//_playerUiParam._hpText->setText(stream.str());
	stream.str("");
	stream.clear(std::stringstream::goodbit);
	stream << "Lv : " << playerParam._level;
	_playerUiParam._lvText->setText(stream.str());
	//stream.str("");
	//stream.clear(std::stringstream::goodbit);
	//stream << "Exp : " << playerParam._exp;
	//_playerUiParam._expText->setText(stream.str());
	stream.str("");
	stream.clear(std::stringstream::goodbit);
	stream << "Next : " << playerParam._nextExp;
	_playerUiParam._nextText->setText(stream.str());

	//CCLOG("nextper:%d", playerParam._nextExpPercent);
	_playerUiParam._nextbar->setPercent(playerParam._nextExpPercent);

	for (int i = playerParam._hp; i < LIFE_MAX; i++) {
		_playerUiParam._lifeImage[i]->setVisible(false);
	}

}

// -----------------------------------------------------------------------------
void GameLayer::applyEnemyLayout()
{
	// 敵レイアウトを更新する
	const std::vector<GameManager::EnemyParam>& enemys = GameManager::getEnemyParam();
	for (auto enemy : enemys) {
		std::stringstream stream;
		stream << "x" << enemy._count;
		int index = enemy._level - 1;
		_enemyUiParam[index]._numText->setText(stream.str());
	}
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
void GameLayer::onPushCallback(Ref* sender, TouchEventType type)
{
	auto select = dynamic_cast<Node*>(sender);
	switch (type) {
    case TOUCH_EVENT_ENDED:
		switch (select->getTag()) {
		case TAG_BUTTON_BOTTOM_CLOSE:
			Director::getInstance()->replaceScene(TransitionFlipX::create(1.0f, TitleScene::createScene()));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
void GameLayer::onGameFinished()
{
	// ゲームが終了した場合はリザルトへ
	Director::getInstance()->replaceScene(TransitionFadeTR::create(1.0f, ResultScene::createScene()));
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
Scene* GameScene::createScene()
{
	// シーンを作成する
	auto scene = Scene::create();
	auto sceneLayer = GameLayer::create();
	auto managerNode = TileManager::create();
	managerNode->setAdapter(sceneLayer);

	// シーンにノードを追加する
	scene->addChild(sceneLayer, 0);
	scene->addChild(managerNode, 1);
	return scene;
}

/******************************************************************************/
//	End Of File
/******************************************************************************/
