/******************************************************************************/
/*
@file	AppResUtil.cpp
@brief	AppResUtil 実装コードです。<br>

@author	Masanari Higuchi
@par	Copyright (C) 2014 Hornet Co., Ltd. All rights reserved.
*/
/******************************************************************************/

#include "AppResUtil.h"
#include <iomanip>

USING_NS_CC;

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
std::string AppResUtil::getGameLevelJsonName(int level)
{
	std::stringstream stream;
	stream << "GameLevel_" << std::setw(2) << std::setfill('0') << level + 1 << ".json";
	return stream.str();
}

// -----------------------------------------------------------------------------
std::string AppResUtil::getNumberTexName(int num)
{
	std::stringstream stream;
	stream << "number_" << num << ".png";
	return stream.str();
}

// -----------------------------------------------------------------------------
std::string AppResUtil::getMosterImage(int num)
{
	std::stringstream stream;
	stream << "monster_" << std::setw(2) << std::setfill('0') << num;
	return stream.str();
}

// -----------------------------------------------------------------------------
std::string AppResUtil::getLifeImage(int num)
{
	std::stringstream stream;
	stream << "Life_" <<  num;
	return stream.str();
}


// -----------------------------------------------------------------------------
std::string AppResUtil::getEnemyTexName(int level)
{
	std::stringstream stream;
	stream << "monster_" << std::setw(2) << std::setfill('0') << level << ".png";
	return stream.str();
}

/******************************************************************************/
//	End Of File
/******************************************************************************/
