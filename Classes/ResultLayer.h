/******************************************************************************/
/*!
@file	ResultLayer.h
@brief	ResultLayer <br>
		リザルトレイヤです。

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/
#ifndef __RESULT_LAYER_H__
#define __RESULT_LAYER_H__

#include "App.h"

////////////////////////////////////////////////////////////////////////////////
/*
	リザルトレイヤ
*/
class ResultLayer : public cocos2d::LayerColor
{
public:
	// ---------------------------------------------------------
	//!@name インスタンス
	//!@{

	// 作成関数
	CREATE_FUNC(ResultLayer);

	//!@}

private:
	// ---------------------------------------------------------
	//!@name 生成/破棄
	//!@{

	// コンストラクタ
	ResultLayer();
	// デストラクタ
	virtual ~ResultLayer() override;

	//!@}
	// ---------------------------------------------------------
	//!@name 基本操作
	//!@{

	// 初期化
	virtual bool init() override;

	void showInterStatial(float dt);

	//!@}
	// ---------------------------------------------------------
	//!@name コールバック
	//!@{

	// プッシュコールバック
	void onPushCallback(cocos2d::Ref* sender, cocos2d::ui::TouchEventType type);

	//!@}
};

////////////////////////////////////////////////////////////////////////////////
/*
	リザルトシーン
*/
class ResultScene
{
public:
	// ---------------------------------------------------------
	//!@name シーン作成
	//!@{

	// シーンを作成する
	static cocos2d::Scene* createScene();

	//!@}
};

#endif	// __RESULT_LAYER_H__
/******************************************************************************/
//	End Of File
/******************************************************************************/
