/******************************************************************************/
/*!
@file	TitleLayer.h
@brief	TitleLayer <br>
		タイトルレイヤです。

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/
#ifndef __TITLE_LAYER_H__
#define __TITLE_LAYER_H__

#include "App.h"

////////////////////////////////////////////////////////////////////////////////
/*
	タイトルレイヤ
*/
class TitleLayer : public cocos2d::LayerColor
{
public:
	// ---------------------------------------------------------
	//!@name インスタンス
	//!@{

	// 作成関数
	CREATE_FUNC(TitleLayer);

	//!@}

private:
	// ---------------------------------------------------------
	//!@name 生成/破棄
	//!@{

	// コンストラクタ
	TitleLayer();
	// デストラクタ
	virtual ~TitleLayer() override;

	//!@}
	// ---------------------------------------------------------
	//!@name 基本操作
	//!@{

	// 初期化
	virtual bool init() override;

	//!@}
	// ---------------------------------------------------------
	//!@name 内部処理
	//!@{

	// 終了
	void gameExit();

	//!@}
	// ---------------------------------------------------------
	//!@name コールバック
	//!@{

	// プッシュコールバック
	void onPushCallback(cocos2d::Ref* sender, cocos2d::ui::TouchEventType type);
	// キーリリース
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	//!@}
};

////////////////////////////////////////////////////////////////////////////////
/*
	タイトルシーン
*/
class TitleScene
{
public:
	// ---------------------------------------------------------
	//!@name シーン作成
	//!@{

	// シーンを作成する
	static cocos2d::Scene* createScene();

	//!@}
};

#endif	// __TITLE_LAYER_H__
/******************************************************************************/
//	End Of File
/******************************************************************************/