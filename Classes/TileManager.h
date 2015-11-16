/******************************************************************************/
/*!
@file	TileManager.h
@brief	TileManager <br>
		タイルを管理する。

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/
#ifndef __TILE_MANAGER_H__
#define __TILE_MANAGER_H__

#include "App.h"
#include "extensions/GUI/CCControlExtension/CCScale9Sprite.h"

////////////////////////////////////////////////////////////////////////////////
/*
	タイル管理
*/
class TileManager : public cocos2d::Node
{
public:
	// ---------------------------------------------------------
	//!@name インスタンス
	//!@{

	// アダプタ
	class Adapter;
	// タイル
	class Tile;
	// 作成
	CREATE_FUNC(TileManager);

	//!@}

private:
	// ---------------------------------------------------------
	//!@name 生成/破棄
	//!@{

	// コンストラクタ
	TileManager();
	// デストラクタ
	virtual ~TileManager();

	//!@}

public:
	// ---------------------------------------------------------
	//!@name アクセサ
	//!@{

	// アダプタを設定する
	void setAdapter(Adapter* adapter) {_adapter = adapter;}

	//!@}

private:
	// ---------------------------------------------------------
	//!@name 内部イベント
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
		SEQ_FINISH,
		SEQ_NUM
	};

	// 実行シーケンス
	void seqRun();
	// 完了シーケンス
	void seqFinish(float dt);

	//!@}
	// ---------------------------------------------------------
	//!@name イベント
	//!@{

	// タッチ開始
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event);
	// タッチ移動中
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event);
	// タッチ終了
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event);
	// エフェクト発生時に呼ばれる
	void onEffect(bool isDamage, bool isLevelup);

	//!@}
	// ---------------------------------------------------------
	//!@name 内部処理
	//!@{

	// タイルを作成する
	void createTile();
	// 敵を作成する
	void createEnemy();
	// タイルを開く
	void openTile(int w, int h);
	// 全てのタイルを開く
	void openTileAll();
	// 向きから差分を算出する
	cocos2d::Point calcDirDelta(Dir dir);
	// 有効なインデックスかチェックする
	bool isEnableIndex(int x, int y);

	//!@}

private:
	Adapter* _adapter;
	Routine _seq;
	Tile*** _tile;
	int _selectIndexX;
	int _selectIndexY;
	cocos2d::Node* _backNode;
	cocos2d::extension::Scale9Sprite* _backFrame;
	cocos2d::Sprite* _effectSpr;
};

////////////////////////////////////////////////////////////////////////////////
/*
	タイル
*/
class TileManager::Tile : public cocos2d::Node
{
	friend TileManager;
public:
	// ---------------------------------------------------------
	//!@name インスタンス
	//!@{

	// 作成
	CREATE_FUNC(Tile);

	//!@}

private:
	// ---------------------------------------------------------
	//!@name 生成/破棄
	//!@{

	// コンストラクタ
	Tile();

	//!@}

public:
	// ---------------------------------------------------------
	//!@name アクセサ
	//!@{

	// 開く
	void open();
	// 表示切り替え
	void dispChange();
	// 番号をセットアップする
	void setupNum(int num);
	// 敵をセットアップする
	void setupEnemy(int level);

	//!@}

private:
	// ---------------------------------------------------------
	//!@name 内部イベント
	//!@{

	// 初期化
	virtual bool init() override;

	//!@}
	// ---------------------------------------------------------
	//!@name 深度
	//!@{

	enum {
		DEPTH_TILE,
		DEPTH_ENEMY,
		DEPTH_NUM,
	};

	//!@}

	cocos2d::extension::Scale9Sprite* _tileSpr;
	cocos2d::Sprite* _enemySpr;
	std::vector<cocos2d::Sprite*> _warnSpr;
	bool _isOpen;
	bool _isEnemyVisible;
	int _warnLevel;
	int _enemyLevel;
};

////////////////////////////////////////////////////////////////////////////////
/*
	アダプタ
*/
class TileManager::Adapter
{
public:
	//! ゲームが終了した時に呼ばれます。
	virtual void onGameFinished() = 0;
};

#endif	// __TILE_MANAGER_H__
/******************************************************************************/
//	End Of File
/******************************************************************************/