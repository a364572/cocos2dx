#pragma once
#include "cocos2d.h"
USING_NS_CC;

enum ENEMY_TYPE {SMALL, MEDIUM, BIG};
class EnemyPlane : public Sprite {

public:
	static EnemyPlane* createEnemy(ENEMY_TYPE enemyType);
	int blood;
	int score;
	ENEMY_TYPE type;
};
