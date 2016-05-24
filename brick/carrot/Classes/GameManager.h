#pragma once
#include "cocos2d.h"
#include "Monster.h"
USING_NS_CC;

/************************************************************************/
/* 管理全局游戏变量资源                                                 */
/************************************************************************/
class GameManager
{
public:
	static GameManager* getInstance();
	Vector<Monster *> monsters;
private:
	static GameManager* _gameManager;
};