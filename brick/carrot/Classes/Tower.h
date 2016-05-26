#pragma once
#include "cocos2d.h"
#include "Monster.h"
USING_NS_CC;


class AbstractTower : public Layer {
public:
	Sprite *towerBase;
	Size visible;
	Point origin;
	Point bulletDirection;
	int flashCount = 0;
	float BULLET_INTERVAL;
	float BULLET_SPEED;

	bool canShoot;
	
	virtual void shootBullet() = 0;
	//virtual void setRotation(float rotation) = 0;
	//virtual float getRotation() = 0;

	static AbstractTower* createTowerByString(std::string name);
	CC_SYNTHESIZE(std::string, towerName, TowerName);
	CC_SYNTHESIZE(std::string, bulletName, BulletName);
	CC_SYNTHESIZE(float, range, Range);
	CC_SYNTHESIZE(float, harm, Harm);
private:
};

/************************************************************************/
/* 每一种炮台都有炮和炮弹两个成员 前者是必须有的，后者随炮台的不同类型变化
/************************************************************************/
class BottleTower : public AbstractTower {

public:
	Vector<Sprite *> bullets;
	static AbstractTower* create();
	virtual void shootBullet();
	virtual void setRotation(float rotation);
	virtual float getRotation();
	virtual void update(float dt);
};



class FanTower : public AbstractTower {
public:
	Sprite *fanBottom;
	Vector<Sprite *> bullets;
	static AbstractTower* create();
	virtual void shootBullet();
	virtual void update(float delta);
};

class FanBullet : public Sprite {
public:
	static FanBullet* create();
	std::vector<Monster *> monsterArr;
	CC_SYNTHESIZE(std::string, bulletName, BulletName);
};