#include "EnemyPlane.h"


EnemyPlane* EnemyPlane::createEnemy(ENEMY_TYPE enemyType) {
	EnemyPlane* sprite = new EnemyPlane();
	if (enemyType == BIG) {
		sprite->initWithFile("big.png");
		sprite->score = 10;
		sprite->blood = 3;
	}
	else if (enemyType == MEDIUM) {
		sprite->initWithFile("medium.png");
		sprite->score = 5;
		sprite->blood = 2;
	}
	else {
		sprite->initWithFile("small.png");
		sprite->score = 2;
		sprite->blood = 1;
	}
	sprite->autorelease();
	sprite->type = enemyType;
	sprite->setAnchorPoint(Vec2(0.5, 1));
	return sprite;
}