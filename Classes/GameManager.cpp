/******************************************************************************/
/*
@file	GameManager.cpp
@brief	GameManager 実装コードです。<br>

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/

#include "GameManager.h"
#include "AppResUtil.h"

USING_NS_CC;
using namespace cocostudio;

// -----------------------------------------------------------------------------
GameManager::GameLevel GameManager::_gameLevel;
GameManager::PlayerParam GameManager::_playerParam;
GameManager::TileParam GameManager::_timeParam;
std::vector<GameManager::PlayerTable> GameManager::_playerTable;
std::vector<GameManager::EnemyParam> GameManager::_enemyParam;
std::vector<GameManager::EnemyTable> GameManager::_enemyTable;
bool GameManager::_isGameClear = false;
bool GameManager::_isGameOver = false;

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
void GameManager::setupGameLevel(GameLevel level)
{
	_gameLevel = level;
	_isGameClear = false;
	_isGameOver = false;

	// ゲームレベルに応じたJSONファイルを読み込む
	std::string fileName = AppResUtil::getGameLevelJsonName(level);
	std::string jsonPath = FileUtils::getInstance()->fullPathForFilename(fileName);
	std::string jsonStr = FileUtils::getInstance()->getStringFromFile(jsonPath);
	rapidjson::Document jsonDict;
	jsonDict.Parse<0>(jsonStr.c_str());

	// タイルパラメータをパースする
	const rapidjson::Value& tileParam = DICTOOL->getSubDictionary_json(jsonDict, "tileParam");
	_timeParam._tileNumX = DICTOOL->getIntValue_json(tileParam, "x");
	_timeParam._tileNumY = DICTOOL->getIntValue_json(tileParam, "y");

	// プレイヤーデータをパースする
	parsePlayerInfo(jsonDict);

	// 敵データをパースする
	parseEnemyInfo(jsonDict);
}

// -----------------------------------------------------------------------------
void GameManager::enemyEncounter(int level, std::function<void(bool, bool)> onEffect)
{
	bool isDamage = false;

	// プレイヤーよりレベルの高い敵の場合はダメージを受ける
	if (_playerParam._level < level) {
		isDamage = true;
		int damage = level - _playerParam._level;
		_playerParam._hp -= damage;
	}
	// プレイヤーのHPが無くなっていたらゲームオーバフラグをOnにする
	if (_playerParam._hp <= 0) {
		onEffect(true, false);
		_isGameOver = true;
		return;
	}

	// 敵を倒す
	for (int i = 0, size = _enemyParam.size(); i < size; ++i) {
		do {
			CC_BREAK_IF(_enemyParam[i]._level != level);
			--_enemyParam[i]._count;
		} while(0);
	}

	// プレイヤーの経験値を増やす
	for (int i = 0, size = _enemyTable.size(); i < size; ++i) {
		do {
			CC_BREAK_IF(_enemyTable[i]._level != level);
			_playerParam._exp += _enemyTable[i]._exp;
		} while(0);
	}
	bool isLevelup = applyPlayerLevel();

	// 全ての敵を倒していたらゲームクリアフラグをOnにする
	int aliveCount = 0;
	for (auto enemy : _enemyParam) {
		aliveCount += enemy._count;
	}
	if (aliveCount == 0) {
		_isGameClear = true;
	}

	// エフェクトを発生させる
	onEffect(isDamage, isLevelup);
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
void GameManager::parsePlayerInfo(const rapidjson::Value& jsonValue)
{
	// プレイヤーパラメータをパースする
	const rapidjson::Value& playerParam = DICTOOL->getSubDictionary_json(jsonValue, "playerParam");
	_playerParam._hp = DICTOOL->getIntValue_json(playerParam, "hp");
	_playerParam._level = 1;
	_playerParam._exp = 0;
	_playerParam._nextExp = 0;
	_playerParam._nextExpPercent = 0;

	// プレイヤーデータをパースする
	int tableSize = DICTOOL->getArrayCount_json(jsonValue, "playerTable");
	_playerTable.resize(tableSize);
	for (int i = 0; i < tableSize; i++) {
		const rapidjson::Value& playerTable = DICTOOL->getDictionaryFromArray_json(jsonValue, "playerTable", i);
		_playerTable[i]._level = DICTOOL->getIntValue_json(playerTable, "level");
		_playerTable[i]._exp = DICTOOL->getIntValue_json(playerTable, "exp");
	}
	for (int i = 0, size = _playerTable.size(); i < size; ++i) {
		if (_playerTable[i]._level == _playerParam._level) {
			_playerParam._nextExp = _playerTable[i]._exp;
		}
	}
}

// -----------------------------------------------------------------------------
void GameManager::parseEnemyInfo(const rapidjson::Value& jsonValue)
{
	// 敵パラメータをパースする
	int paramSize = DICTOOL->getArrayCount_json(jsonValue, "enemyParam");
	_enemyParam.resize(paramSize);
	for (int i = 0; i < paramSize; i++) {
		const rapidjson::Value& enemyParam = DICTOOL->getDictionaryFromArray_json(jsonValue, "enemyParam", i);
		_enemyParam[i]._level = DICTOOL->getIntValue_json(enemyParam, "level");
		_enemyParam[i]._count = DICTOOL->getIntValue_json(enemyParam, "count");
	}
	// 敵データをパースする
	int tableSize = DICTOOL->getArrayCount_json(jsonValue, "enemyTable");
	_enemyTable.resize(tableSize);
	for (int i = 0; i < tableSize; i++) {
		const rapidjson::Value& enemyTable = DICTOOL->getDictionaryFromArray_json(jsonValue, "enemyTable", i);
		_enemyTable[i]._level = DICTOOL->getIntValue_json(enemyTable, "level");
		_enemyTable[i]._exp = DICTOOL->getIntValue_json(enemyTable, "exp");
	}
}

// -----------------------------------------------------------------------------
bool GameManager::applyPlayerLevel()
{
	int maxLevel = 1;
	int nowLevel = -1;
	int nextExp  = 9999;

	int diffExp  = 0;
	for (int i = 0, size = _playerTable.size(); i < size; ++i) {
		do {
			if (_playerTable[i]._exp == -1) {
				maxLevel = _playerTable[i]._level;
				break;
			}
			diffExp  = _playerTable[i]._exp - diffExp;
			CC_BREAK_IF(_playerTable[i]._exp <= _playerParam._exp);
			int delta = _playerTable[i]._exp - _playerParam._exp;
			CC_BREAK_IF(delta >= nextExp);
			nextExp = delta;
			nowLevel = _playerTable[i]._level;
		} while(0);
	}

	// 見つからなかった場合は最大レベル
	if (nowLevel == -1) {
		nowLevel = maxLevel;
		nextExp = 0;
		_playerParam._nextExpPercent = 100;
	} else {
		// ゲージの位置を求める
		int p = diffExp - nextExp;
		_playerParam._nextExpPercent = ((diffExp - nextExp) * 100) / diffExp;
	}

	_playerParam._nextExp = nextExp;
	bool isLevelup = false;
	if (_playerParam._level != nowLevel) {
		_playerParam._level = nowLevel;
		isLevelup = true;
	}
	return isLevelup;
}

/******************************************************************************/
//	End Of File
/******************************************************************************/
