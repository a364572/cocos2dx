#pragma once
#include "cocos2d.h"
#include "Monster.h"
USING_NS_CC;


class AbstractBullet : public Sprite
{
public:

	//virtual void checkCollision() = 0;					//���������Ƿ���ײ
	std::vector<Monster *> monsterArr;						//�뵱ǰ�ӵ��Ѿ��Ӵ����Ĺ����б�
	CC_SYNTHESIZE(bool, avaliable, Avaliable);				//�ж��ӵ��Ƿ�ʧЧ
	CC_SYNTHESIZE(float, speed, Speed);						//�ӵ��˶��ٶ�
};

/************************************************************************/
/* ÿһ����̨�����ں��ڵ�������Ա ǰ���Ǳ����еģ���������̨�Ĳ�ͬ���ͱ仯
/************************************************************************/
class AbstractTower : public Sprite {
public:
	Size visible;											//��Ļ��������
	Point origin;											//��Ļ���
	int flashCount = 0;
	Vector<AbstractBullet *> bulletArr;						//������ӵ��б�						
	
	virtual void shootBullet() = 0;

	static AbstractTower* createTowerByString(std::string name);
	CC_SYNTHESIZE(std::string, towerName, TowerName);		//��̨����
	CC_SYNTHESIZE(std::string, bulletName, BulletName);		//�ӵ�����
	CC_SYNTHESIZE(bool, canShoot, CanShoot);				//�жϵ�ǰ�Ƿ�������
	CC_SYNTHESIZE(float, range, Range);						//��̨������Χ
	CC_SYNTHESIZE(float, harm, Harm);						//ÿ�ε��˺�
	CC_SYNTHESIZE(float, bulletInterval, BulletInterval);	//�ӵ�������
	CC_SYNTHESIZE(float, bulletSpeed, BulletSpeed);
	CC_SYNTHESIZE(Point, towerDirection, TowerDirection);	//����̨������ת
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
