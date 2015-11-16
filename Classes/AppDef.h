/******************************************************************************/
/*!
@file	AppDef.h
@brief	AppDef <br>
		アプリケーション共通の定義です。

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/
#ifndef __APP_DEF_H__
#define __APP_DEF_H__

enum Dir {
	DIR_T = 0,
	DIR_TR,
	DIR_R,
	DIR_BR,
	DIR_B,
	DIR_BL,
	DIR_L,
	DIR_TL,
	DIR_NUM
};

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
#define NEND_API_KEY "a6eca9dd074372c898dd1df549301f277c53f2b9"
#define NEND_SPOT_ID "3172"
#define NEND_INTERSTITIAL_API_KEY "308c2499c75c4a192f03c02b2fcebd16dcb45cc9"
#define NEND_INTERSTITIAL_SPOT_ID "213208"
#else
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define NEND_API_KEY "e95df0f3dd6125cd4f68a0c2e96388a214816bde"
#define NEND_SPOT_ID "193183"
#define NEND_INTERSTITIAL_API_KEY "3f93056d0121505fe81e4ba62f5789dfbe1fbdab"
#define NEND_INTERSTITIAL_SPOT_ID "225961"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define NEND_API_KEY "5b961492ea89b95ca54850ba1776d99f389e539d"
#define NEND_SPOT_ID "198411"
#define NEND_INTERSTITIAL_API_KEY "c885c2cf6160a5ec6924dbd0b56e089445aea152"
#define NEND_INTERSTITIAL_SPOT_ID "225962"
#else
#define NEND_API_KEY ""
#define NEND_SPOT_ID ""
#define NEND_INTERSTITIAL_API_KEY ""
#define NEND_INTERSTITIAL_SPOT_ID ""
#endif
#endif

#endif	// __APP_DEF_H__
/******************************************************************************/
//	End Of File
/******************************************************************************/
