/******************************************************************************/
/*!
@file	AppDelegate.h
@brief	AppDelegate <br>
		アプリケーションの共通処理を行う。

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/
#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "App.h"

////////////////////////////////////////////////////////////////////////////////
/*
	アプリケーション共通処理
*/
class  AppDelegate : private cocos2d::Application
{
public:
	// ---------------------------------------------------------
	//!@name 生成/破棄
	//!@{

	// コンストラクタ
    AppDelegate();
	// デストラクタ
    virtual ~AppDelegate() override;

	//!@}
	// ---------------------------------------------------------
	//!@name ApplicationProtocol オーバーライド
	//!@{

	// アプリケーションの起動時に呼び出されます。
    virtual bool applicationDidFinishLaunching() override;
	// アプリケーションがバックグラウンドに入る時に呼び出されます。
    virtual void applicationDidEnterBackground() override;
	// アプリケーションがフォアグラウンドに戻る時に呼び出されます。
    virtual void applicationWillEnterForeground() override;

	//!@}
};

#endif // _APP_DELEGATE_H_
/******************************************************************************/
//	End Of File
/******************************************************************************/