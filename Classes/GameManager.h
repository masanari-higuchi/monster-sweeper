/******************************************************************************/
/*!
@file	GameManager.h
@brief	GameManager <br>
		ゲームマネージャです。

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/
#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "App.h"
#include "cocostudio/DictionaryHelper.h"

////////////////////////////////////////////////////////////////////////////////
/*
	ゲームマネージャ
*/
class GameManager
{
public:
	// ---------------------------------------------------------
	//!@name ゲームレベル
	//!@{

	enum GameLevel {
		GAME_LEVEL_EASY,
		GAME_LEVEL_NORMAL,
		GAME_LEVEL_HARD,
		GAME_LEVEL_NUM
	};

	//!@}
	// ---------------------------------------------------------
	//!@name パラメータ
	//!@{

	struct PlayerParam {
		int _hp;
		int _level;
		int _exp;
		int _nextExp;
		int _nextExpPercent;
	};
	struct TileParam {
		int _tileNumX;
		int _tileNumY;
	};
	struct EnemyParam {
		int _level;
		int _count;
	};

	//!@}
	// ---------------------------------------------------------
	//!@name アクセサ
	//!@{

	// ゲームレベルを取得する
	static GameLevel getGameLevel() {return _gameLevel;}
	// ゲームレベルをセットアップする
	static void setupGameLevel(GameLevel level);
	// プレイヤーパラメータを取得する
	static const PlayerParam& getPlayerParam() {return _playerParam;}
	// タイルパラメータを取得する
	static const TileParam& getTileParam() {return _timeParam;}
	// 敵パラメータを取得する
	static const std::vector<EnemyParam>& getEnemyParam() {return _enemyParam;}
	// 敵と遭遇した
	static void enemyEncounter(int level, std::function<void(bool, bool)> onEffect);
	// ゲームクリアかチェックする
	static bool isGameClear() {return _isGameClear;}
	// ゲームオーバーかチェックする
	static bool isGameOver() {return _isGameOver;}

	//!@}

private:
	// ---------------------------------------------------------
	//!@name 内部パラメータ
	//!@{

	struct PlayerTable {
		int _level;
		int _exp;
	};
	struct EnemyTable {
		int _level;
		int _exp;
	};

	//!@}
	// ---------------------------------------------------------
	//!@name パース
	//!@{

	// プレイヤーデータをパースする
	static void parsePlayerInfo(const rapidjson::Value& jsonValue);
	// 敵データをパースする
	static void parseEnemyInfo(const rapidjson::Value& jsonValue);
	// プレイヤーレベルを更新する
	static bool applyPlayerLevel();

	//!@}

	static GameLevel _gameLevel;
	static PlayerParam _playerParam;
	static TileParam _timeParam;
	static std::vector<PlayerTable> _playerTable;
	static std::vector<EnemyParam> _enemyParam;
	static std::vector<EnemyTable> _enemyTable;
	static bool _isGameClear;
	static bool _isGameOver;
};

#endif	// __GAME_MANAGER_H__
/******************************************************************************/
//	End Of File
/******************************************************************************/
