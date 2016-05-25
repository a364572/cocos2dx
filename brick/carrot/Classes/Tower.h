#pragma once
#include "cocos2d.h"
USING_NS_CC;


class AbstractTower : public Layer {
public:
	Sprite *towerBase;
	Size visible;
	Point origin;
	int flashCount = 0;
	float BULLET_INTERVAL;
	float BULLET_SPEED;
	float bullet_direction;
	bool canShoot;
	
	Point getVectorOfTowerBase();
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
/* ÿһ����̨�����ں��ڵ�������Ա ǰ���Ǳ����еģ���������̨�Ĳ�ͬ���ͱ仯
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
	Vector<Sprite *> bullets;
	static AbstractTower* create();
	virtual void shootBullet();
	virtual void update(float delta);
};