#pragma once
#include "cocos2d.h"
#include "Monster.h"
USING_NS_CC;


class AbstractBullet : public Sprite
{
public:

	//virtual void checkCollision() = 0;					//检查与怪物是否碰撞
	std::vector<Monster *> monsterArr;						//与当前子弹已经接触过的怪物列表
	CC_SYNTHESIZE(bool, avaliable, Avaliable);				//判断子弹是否失效
	CC_SYNTHESIZE(float, speed, Speed);						//子弹运动速度
};

/************************************************************************/
/* 每一种炮台都有炮和炮弹两个成员 前者是必须有的，后者随炮台的不同类型变化
/************************************************************************/
class AbstractTower : public Sprite {
public:
	Size visible;											//屏幕可视区域
	Point origin;											//屏幕起点
	int flashCount = 0;
	Vector<AbstractBullet *> bulletArr;						//发射的子弹列表						
	
	virtual void shootBullet() = 0;

	static AbstractTower* createTowerByString(std::string name);
	CC_SYNTHESIZE(std::string, towerName, TowerName);		//炮台名称
	CC_SYNTHESIZE(std::string, bulletName, BulletName);		//子弹名称
	CC_SYNTHESIZE(bool, canShoot, CanShoot);				//判断当前是否可以射击
	CC_SYNTHESIZE(float, range, Range);						//炮台攻击范围
	CC_SYNTHESIZE(float, harm, Harm);						//每次的伤害
	CC_SYNTHESIZE(float, bulletInterval, BulletInterval);	//子弹发射间隔
	CC_SYNTHESIZE(float, bulletSpeed, BulletSpeed);
	CC_SYNTHESIZE(Point, towerDirection, TowerDirection);	//该炮台可以旋转
private:
};

class BottleTower : public AbstractTower {

public:
	static AbstractTower* create();
	virtual void shootBullet();
	virtual void update(float dt);

};

class BottleBullet : public AbstractBullet{
public:
	static AbstractBullet* create(std::string name);
	//virtual void checkCollision();
};

class FanTower : public AbstractTower {
public:
	static AbstractTower* create();
	virtual void shootBullet();
	virtual void update(float delta);
};

class FanBullet : public AbstractBullet {
public:
	static AbstractBullet* create(std::string name);
	//virtual void checkCollision();
};
