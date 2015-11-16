/******************************************************************************/
/*!
@file	AppResUtil.h
@brief	AppResUtil <br>
		アプリケーションリソース関連のユーティリティです。

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/
#ifndef __APP_RES_UTIL_H__
#define __APP_RES_UTIL_H__

#include "App.h"

////////////////////////////////////////////////////////////////////////////////
/*
	リソースユーティリティ
*/
class AppResUtil
{
public:
	// ---------------------------------------------------------
	//!@name アクセサ
	//!@{

	// 難易度別JSON名を取得する
	static std::string getGameLevelJsonName(int level);
	// 番号テクスチャ名を取得する
	static std::string getNumberTexName(int num);
	// モンスターのイメージ名を取得する
	static std::string getMosterImage(int num);
	// ライフの名前を取得する
	static std::string getLifeImage(int num);
	// 敵テクスチャ名を取得する
	static std::string getEnemyTexName(int level);

	//!@}
};

#endif	// __APP_RES_UTIL_H__
/******************************************************************************/
//	End Of File
/******************************************************************************/
