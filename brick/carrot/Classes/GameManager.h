#pragma once
#include "cocos2d.h"
#include "Monster.h"
USING_NS_CC;

/************************************************************************/
/* ����ȫ����Ϸ������Դ                                                 */
/************************************************************************/
class GameManager
{
public:
	static GameManager* getInstance();
	Vector<Monster *> monsters;
private:
	static GameManager* _gameManager;
};