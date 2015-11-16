/******************************************************************************/
/*!
@file	GameLayer.h
@brief	GameLayer <br>
		ゲームレイヤです。

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/
#ifndef __GAME_LAYER_H__
#define __GAME_LAYER_H__

#include "App.h"
#include "TileManager.h"

////////////////////////////////////////////////////////////////////////////////
/*
	ゲームレイヤ
*/
class GameLayer : public cocos2d::LayerColor
				, public TileManager::Adapter
{
public:
	// ---------------------------------------------------------
	//!@name インスタンス
	//!@{

	// 敵の種類
	static const int ENEMY_NUM = 9;
	static const int LIFE_MAX  = 10;
	// 作成関数
	CREATE_FUNC(GameLayer);

	//!@}

private:
	// ---------------------------------------------------------
	//!@name 生成/破棄
	//!@{

	// コンストラクタ
	GameLayer();
	// デストラクタ
	virtual ~GameLayer() override;

	//!@}
	// ---------------------------------------------------------
	//!@name 基本操作
	//!@{

	// 初期化
	virtual bool init() override;
	// 更新
	void update(float dt);

	//!@}
	// ---------------------------------------------------------
	//!@name シーケンス
	//!@{

	enum {
		SEQ_RUN,
		SEQ_NUM
	};

	// 実行シーケンス
	void seqRun();

	//!@}
	// ---------------------------------------------------------
	//!@name 内部処理
	//!@{

	// プレイヤーレイアウトを作成する
	void initPlayerLayout(cocos2d::ui::Widget* layout);
	void createPlayerLayout();
	// 敵レイアウトを作成する
	void initEnemyLayout(cocos2d::ui::Widget* layout);
	void createEnemyLayout();
	// 下部レイアウトを作成する
	void createBottomLayout();
	// プレイヤーレイアウトを更新する
	void applyPlayerLayout();
	// 敵レイアウトを更新する
	void applyEnemyLayout();

	//!@}
	// ---------------------------------------------------------
	//!@name コールバック
	//!@{

	// プッシュコールバック
	void onPushCallback(cocos2d::Ref* sender, cocos2d::ui::TouchEventType type);

	//!@}
	// ---------------------------------------------------------
	//!@name TileManager::Adapter オーバーライド
	//!@{

	//! ゲームが終了した時に呼ばれます。
	virtual void onGameFinished() override;

	//!@}
	// ---------------------------------------------------------
	//!@name UIパラメータ
	//!@{

	struct PlayerUiParam {
		cocos2d::ui::Text* _hpText;
		cocos2d::ui::Text* _lvText;
		cocos2d::ui::Text* _expText;
		cocos2d::ui::Text* _nextText;
		cocos2d::ui::LoadingBar* _nextbar;
		cocos2d::ui::ImageView*  _lifeImage[LIFE_MAX];
		PlayerUiParam() : _hpText(nullptr), _lvText(nullptr), _expText(nullptr), _nextText(nullptr), _nextbar(nullptr) {}
	};
	struct EnemyUiParam {
		cocos2d::ui::Text* _lvText;
		cocos2d::ui::Text* _numText;
		cocos2d::ui::ImageView* _image;
		bool _isUse;
		EnemyUiParam() : _lvText(nullptr), _numText(nullptr), _image(nullptr), _isUse(false) {}
	};

	//!@}

	Routine			_seq;
	PlayerUiParam	_playerUiParam;
	EnemyUiParam	_enemyUiParam[ENEMY_NUM];
};

////////////////////////////////////////////////////////////////////////////////
/*
	ゲームシーン
*/
class GameScene
{
public:
	// ---------------------------------------------------------
	//!@name シーン作成
	//!@{

	// シーンを作成する
	static cocos2d::Scene* createScene();

	//!@}
};

#endif	// __GAME_LAYER_H__
/******************************************************************************/
//	End Of File
/******************************************************************************/
