/******************************************************************************/
/*
@file	TileManager.cpp
@brief	TileManager 実装コードです。<br>

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/

#include "TileManager.h"
#include "GameManager.h"
#include "AppResUtil.h"

USING_NS_CC;
using namespace CocosDenshion;

// -----------------------------------------------------------------------------
namespace
{
	Size TILE_SIZE(56.0f, 56.0f);
	Size TILE_TEX_SIZE(50.0f, 50.0f);
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
TileManager::TileManager()
	:	_adapter(nullptr)
	,	_seq(SEQ_NUM)
	,	_tile(nullptr)
	,	_selectIndexX(-1)
	,	_selectIndexY(-1)
	,	_backNode(nullptr)
	,	_backFrame(nullptr)
	,	_effectSpr(nullptr)
{
	// 乱数のシードを設定する
	srand(time(0));

	// タイルを作成する
	const GameManager::TileParam& param = GameManager::getTileParam();
	_tile = new Tile**[param._tileNumX];
	for (int i = 0, size = param._tileNumX; i < size; ++i) {
		_tile[i] = new Tile*[param._tileNumY];
	}
}

// -----------------------------------------------------------------------------
TileManager::~TileManager()
{
	// タイルを破棄する
	const GameManager::TileParam& param = GameManager::getTileParam();
	for (int i = 0, size = param._tileNumX; i < size; ++i) {
		CC_SAFE_DELETE_ARRAY(_tile[i]);
	}
	CC_SAFE_DELETE_ARRAY(_tile);
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
bool TileManager::init()
{
	const GameManager::TileParam& param = GameManager::getTileParam();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	// 自身の位置を画面中央に持ってくる
	Size backSize(TILE_SIZE.width * param._tileNumX, TILE_SIZE.height * param._tileNumY);
	Point center(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y - 160);

	// 背景フレームを作成する
	_backFrame = extension::Scale9Sprite::create(Rect(12, 12, 6, 6), "CommonFrame_03.png");
	_backFrame->setPosition(center);
	_backFrame->setContentSize(backSize + Size(12, 12));
	this->addChild(_backFrame, 1);

	// 背景ノードを作成する
	_backNode = Node::create();
	_backNode->setPosition(center - Point(backSize / 2));
	this->addChild(_backNode, 1);

	// エフェクトスプライトを作成する
	_effectSpr = Sprite::create();
	_effectSpr->setAnchorPoint(Point::ZERO);
	_effectSpr->setTextureRect(Rect(origin.x, origin.y, visibleSize.width, visibleSize.height));
	_effectSpr->setOpacity(0);
	this->addChild(_effectSpr, 0);

	// タイルを作成する
	this->createTile();

	// 更新イベントを登録する
	this->scheduleUpdate();

	// シングルタッチイベントを登録する
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(TileManager::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(TileManager::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(TileManager::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _backNode);
	return true;
}

// -----------------------------------------------------------------------------
void TileManager::update(float dt)
{
	switch (_seq.getNo()) {
	case SEQ_RUN:
		seqRun();
		break;
	case SEQ_FINISH:
		seqFinish(dt);
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
void TileManager::seqRun()
{
	// ゲームクリアの場合はリザルトへ
	if (GameManager::isGameClear() || GameManager::isGameOver()) {
		_seq.setNo(SEQ_FINISH);
	}
}

// -----------------------------------------------------------------------------
void TileManager::seqFinish(float dt)
{
	enum {
		STEP_TILE_OPEN = 0,
		STEP_TILE_WAIT = 10,
		STEP_FINISH = 20
	};
	switch (_seq._step) {
	case STEP_TILE_OPEN:
		_seq._time += dt;
		if (_seq._time < 1.5f) {
			break;
		}
		this->openTileAll();
		_seq._time = 0.0f;
		_seq._step = STEP_TILE_WAIT;
		break;
	case STEP_TILE_WAIT:
		_seq._time += dt;
		if (_seq._time < 1.5f) {
			break;
		}
		if (_adapter != nullptr) {
			_adapter->onGameFinished();
		}
		_seq._step = STEP_FINISH;
		break;
	case STEP_FINISH:
		break;
	default:
		break;
	}

}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
bool TileManager::onTouchBegan(Touch* touch, Event* unused_event)
{
	if (_seq.getNo() == SEQ_FINISH) {
		return false;
	}
	// 選択インデックスを設定する
	auto target = unused_event->getCurrentTarget();
	Point touchPos = target->convertToNodeSpace(touch->getLocation());
	int x = touchPos.x / TILE_SIZE.width;
	int y = touchPos.y / TILE_SIZE.height;
	do {
		CC_BREAK_IF(!this->isEnableIndex(x, y));
		_selectIndexX = x;
		_selectIndexY = y;
	} while(0);
	return true;
}

// -----------------------------------------------------------------------------
void TileManager::onTouchMoved(Touch* touch, Event* unused_event)
{
}

// -----------------------------------------------------------------------------
void TileManager::onTouchEnded(Touch* touch, Event* unused_event)
{
	// 選択されたタイルを開く
	auto target = unused_event->getCurrentTarget();
	Point touchPos = target->convertToNodeSpace(touch->getLocation());
	int x = touchPos.x / TILE_SIZE.width;
	int y = touchPos.y / TILE_SIZE.height;
	do {
		CC_BREAK_IF(_selectIndexX != x || _selectIndexY != y);
		CC_BREAK_IF(!this->isEnableIndex(x, y));
		this->openTile(x, y);
	} while(0);
}

// -----------------------------------------------------------------------------
void TileManager::onEffect(bool isDamage, bool isLevelup)
{
	Vector<FiniteTimeAction*> actions;

	// 画面を揺らす
	if (isDamage) {
		float POWER = 8.0f;
		actions.pushBack(MoveTo::create(0.025f, Point(POWER, POWER)));
		actions.pushBack(MoveTo::create(0.025f, Point(0, 0)));
		actions.pushBack(MoveTo::create(0.025f, Point(-POWER, POWER)));
		actions.pushBack(MoveTo::create(0.025f, Point(0, 0)));
		actions.pushBack(MoveTo::create(0.025f, Point(-POWER, -POWER)));
		actions.pushBack(MoveTo::create(0.025f, Point(0, 0)));
		actions.pushBack(MoveTo::create(0.025f, Point(POWER, -POWER)));
		actions.pushBack(MoveTo::create(0.025f, Point(0, 0)));
		auto scene = Director::getInstance()->getRunningScene();
		scene->runAction(Sequence::create(actions));
	}

	// エフェクトスプライトを点滅させる
	if (isLevelup) {
		actions.clear();
		actions.pushBack(FadeTo::create(0.1f, 80));
		actions.pushBack(FadeTo::create(0.1f, 0));
		_effectSpr->setColor(Color3B(50, 255, 50));
		_effectSpr->runAction(Sequence::create(actions));
	}
	else if (isDamage) {
		actions.clear();
		actions.pushBack(FadeTo::create(0.025f, 80));
		actions.pushBack(FadeTo::create(0.025f, 0));
		actions.pushBack(FadeTo::create(0.025f, 80));
		actions.pushBack(FadeTo::create(0.025f, 0));
		_effectSpr->setColor(Color3B(255, 50, 50));
		_effectSpr->runAction(Sequence::create(actions));
		SimpleAudioEngine::getInstance()->playEffect("damage.mp3");
	}
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
void TileManager::createTile()
{
	// タイルを作成する
	const GameManager::TileParam& param = GameManager::getTileParam();
	Point offset(TILE_SIZE * 0.5f);
	for (int w = 0, wSize = param._tileNumX; w < wSize; ++w) {
		for (int h = 0, hSize = param._tileNumY; h < hSize; ++h) {
			Tile*& tile = _tile[w][h];
			Point pos(TILE_SIZE.width * w, TILE_SIZE.height * h);
			tile = Tile::create();
			tile->setAnchorPoint(Point::ANCHOR_MIDDLE);
			tile->setPosition(pos + offset);
			_backNode->addChild(tile, 0);
		}
	}
	// 敵を作成する
	this->createEnemy();

	// 警告レベルを設定する
	for (int w = 0, wSize = param._tileNumX; w < wSize; ++w) {
		for (int h = 0, hSize = param._tileNumY; h < hSize; ++h) {
			Tile*& tile = _tile[w][h];
			int sumLevel = 0;
			for (int dir = 0; dir < DIR_NUM; ++dir) {
				Point delta = this->calcDirDelta(static_cast<Dir>(dir));
				int x = static_cast<int>(delta.x) + w;
				int y = static_cast<int>(delta.y) + h;
				if (!this->isEnableIndex(x, y)) {
					continue;
				}
				if (_tile[x][y]->_enemyLevel == -1) {
					continue;
				}
				sumLevel += _tile[x][y]->_enemyLevel;
			}
			tile->_warnLevel = sumLevel;
			tile->setupNum(sumLevel);
		}
	}
}

// -----------------------------------------------------------------------------
void TileManager::createEnemy()
{
	// インデックス配列を作成する
	const GameManager::TileParam& tileParam = GameManager::getTileParam();
	int tileNum = tileParam._tileNumX * tileParam._tileNumY;
	std::vector<int> indexTbl(tileNum);
	for (int i = 0; i < tileNum; ++i) {
		indexTbl[i] = i;
	}

	// 敵を設定する
	const std::vector<GameManager::EnemyParam>& enemyParam = GameManager::getEnemyParam();
	for (int i = 0, iSize = enemyParam.size(); i < iSize; ++i) {
		for (int j = 0, jSize = enemyParam[i]._count; j < jSize; ++j) {
			int enemyLevel = enemyParam[i]._level;
			int select = rand() % indexTbl.size();
			int index = indexTbl[select];
			indexTbl.erase(indexTbl.begin() + select);
			int x = index % tileParam._tileNumX;
			int y = index / tileParam._tileNumX;
			Tile*& tile = _tile[x][y];
			tile->setupEnemy(enemyLevel);
		}
	}
}

// -----------------------------------------------------------------------------
void TileManager::openTile(int w, int h)
{
	// 既に開いている場合は表示を切り替える
	auto tile = _tile[w][h];
	if(tile->_isOpen) {
		tile->dispChange();
		return;
	}
	tile->open();

	// 敵がいた場合はエンカウントする
	if (tile->_enemyLevel != -1) {
		GameManager::enemyEncounter(tile->_enemyLevel, CC_CALLBACK_2(TileManager::onEffect, this));
		return;
	}

	// 警告レベルが0の場合は再帰的に周囲のタイルを開く
	do {
		CC_BREAK_IF(tile->_warnLevel != 0);
		for (int dir = 0; dir < DIR_NUM; ++dir) {
			Point delta = this->calcDirDelta(static_cast<Dir>(dir));
			int x = static_cast<int>(delta.x) + w;
			int y = static_cast<int>(delta.y) + h;
			if (!this->isEnableIndex(x, y)) {
				continue;
			}
			this->openTile(x, y);
		}
	} while(0);
}

// -----------------------------------------------------------------------------
void TileManager::openTileAll()
{
	// 全てのタイルを開く
	const GameManager::TileParam& param = GameManager::getTileParam();
	for (int w = 0, wSize = param._tileNumX; w < wSize; ++w) {
		for (int h = 0, hSize = param._tileNumY; h < hSize; ++h) {
			Tile*& tile = _tile[w][h];
			tile->open();
		}
	}
}

// -----------------------------------------------------------------------------
Point TileManager::calcDirDelta(Dir dir)
{
	switch(dir) {
	case DIR_T:
		return Point(0, 1);
	case DIR_TR:
		return Point(1, 1);
	case DIR_R:
		return Point(1, 0);
	case DIR_BR:
		return Point(1, -1);
	case DIR_B:
		return Point(0, -1);
	case DIR_BL:
		return Point(-1, -1);
	case DIR_L:
		return Point(-1, 0);
	case DIR_TL:
		return Point(-1, 1);
	default:
		break;
	}
	return Point(-1, -1);
}

// -----------------------------------------------------------------------------
bool TileManager::isEnableIndex(int x, int y)
{
	const GameManager::TileParam& param = GameManager::getTileParam();
	do {
		CC_BREAK_IF(x < 0 || x >= param._tileNumX);
		CC_BREAK_IF(y < 0 || y >= param._tileNumY);
		return true;
	} while(0);
	return false;
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
TileManager::Tile::Tile()
	:	_tileSpr(nullptr)
	,	_enemySpr(nullptr)
	,	_isOpen(false)
	,	_isEnemyVisible(false)
	,	_warnLevel(0)
	,	_enemyLevel(-1)
{
}

// -----------------------------------------------------------------------------
bool TileManager::Tile::init()
{
	// タイル情報を作成する
	auto& tileSpr = _tileSpr;
	tileSpr = extension::Scale9Sprite::create(Rect(12, 12, 6, 6), "CommonFrame_04.png");
	tileSpr->setContentSize(TILE_TEX_SIZE);
	tileSpr->setOpacity(200);
	this->addChild(tileSpr, DEPTH_TILE);
	return true;
}

// -----------------------------------------------------------------------------
void TileManager::Tile::open()
{
	// タイルを開く
	if (!_isOpen) {
		Vector<FiniteTimeAction*> actions;
		actions.pushBack(FadeTo::create(0.25f, 100));
		_tileSpr->runAction(Sequence::create(actions));
		this->dispChange();
		_isOpen = true;
	}
}

// -----------------------------------------------------------------------------
void TileManager::Tile::dispChange()
{
	// 敵がいる場合は表示を切り替える
	if (_enemySpr != nullptr) {
		_isEnemyVisible = !_isEnemyVisible;
		_enemySpr->setVisible(_isEnemyVisible);
		for (auto spr : _warnSpr) {
			spr->setVisible(!_isEnemyVisible);
		}
	} else {
		for (auto spr : _warnSpr) {
			spr->setVisible(true);
		}
	}
}

// -----------------------------------------------------------------------------
void TileManager::Tile::setupNum(int num)
{
	// 敵がいる場合は0も表示する
	if (num == 0 && _enemyLevel == -1) {
		return;
	}
	// 桁数分のスプライトを作成する
	int digit = num > 0 ? static_cast<int>(log10(num)) + 1 : 1;
	float offsetWidth = 0.0f;
	for (int i = 0, tmpNum = num; i < digit; ++i, tmpNum /= 10) {
		int numSplit = tmpNum % 10;
		std::string texName = AppResUtil::getNumberTexName(numSplit);
		Sprite* numSpr = Sprite::create(texName);
		if (_enemyLevel != -1) {
			numSpr->setColor(Color3B::RED);
		}
		numSpr->setVisible(false);
		this->addChild(numSpr, DEPTH_NUM);
		_warnSpr.push_back(numSpr);
		offsetWidth -= numSpr->getContentSize().width * 0.5f;
	}
	// 2桁以上の場合は位置を調整する
	if (digit <= 1) {
		return;
	}
	for (int i = digit - 1; i >= 0; --i) {
		float contentWidth = _warnSpr[i]->getContentSize().width;
		Point pos(offsetWidth + contentWidth * 0.5f, 0.0f);
		_warnSpr[i]->setPosition(pos);
		offsetWidth += contentWidth;
	}
}

// -----------------------------------------------------------------------------
void TileManager::Tile::setupEnemy(int level)
{
	// 敵をセットアップする
	_enemyLevel = level;
	std::string texName = AppResUtil::getEnemyTexName(level);
	_enemySpr = Sprite::create(texName);
	_enemySpr->setVisible(false);
	this->addChild(_enemySpr, DEPTH_ENEMY);
}

/******************************************************************************/
//	End Of File
/******************************************************************************/
