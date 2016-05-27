#include "Tower.h"
#include "GameManager.h"

AbstractTower* BottleTower::create()
{
	BottleTower* tower = new BottleTower();
	tower->setCanShoot(false);
	tower->setBulletInterval(1.0f);
	tower->setRange(200);
	tower->setHarm(20);
	tower->setTowerName("Bottle.png");
	tower->setBulletName("Bottle-bullet.png");
	tower->setTowerDirection(Point(0, 0));
	tower->initWithSpriteFrameName(tower->getTowerName());
	tower->setAnchorPoint(Size(0.5, 0.5));
	tower->autorelease(); 
	return tower;
}

AbstractBullet* BottleBullet::create(std::string name)
{
	auto bullet = new BottleBullet();
	bullet->setAvaliable(true);
	bullet->setSpeed(200);
	bullet->initWithSpriteFrameName("name");
	bullet->setAnchorPoint(Vec2(0.5, 0.5));
	bullet->setOpacity(255);
	return bullet;
}

void BottleTower::shootBullet() 
{
	auto bullet = BottleBullet::create(this->getBulletName());
	bullet->setPosition(this->getPosition() + this->getTowerDirection() * 5);
	
	Vec2 direction = this->getTowerDirection() * 1000;
	//这里还可以加个碰撞后爆炸的动作
	bullet->runAction(Sequence::create(MoveBy::create(bullet->getPosition().distance(direction) / bullet->getSpeed(), direction),
		CallFunc::create([=] {bullet->setAvaliable(false);}), NULL));
	this->bulletArr.pushBack(bullet);
	this->addChild(bullet);
}

void BottleTower::update(float dt) {

	//检查炮台的攻击范围内是否有怪物 有的话先旋转角度后射击
	this->setCanShoot(false);
	auto monsters = GameManager::getInstance()->monsters;
	for (int i = 0; i < monsters.size(); i++)
	{
		auto monster = monsters.at(i);
		if (monster->getBlood() > 0)
		{
			Point point = this->convertToNodeSpace(monster->convertToWorldSpace(Vec2(0, 0)));	//首先将怪物的坐标转换为炮台的坐标
			auto distance = point.distance(this->getPosition());								//怪物到炮台的距离
			
			//稍后补上
			if (distance <= getRange())
			{
				this->setCanShoot(true);
				break;
			}
		}
	}

	//检查是否要发射子弹
	flashCount++;
	if (flashCount * Director::getInstance()->getAnimationInterval() > this->getBulletInterval() && canShoot)
	{
		flashCount = 0;
		shootBullet();
	}

	//检查子弹是否出界以及检查子弹和怪物是否碰撞
	
	for (int i = 0; i < bulletArr.size(); i++) 
	{
		auto bullet = (BottleBullet *)bulletArr.at(i);
		//清除子弹
		if (bullet->getPositionX() - origin.x < 0 || bullet->getPositionX() - origin.x > visible.width  ||
			bullet->getPositionY() - origin.y < 0 || bullet->getPositionY() - origin.y > visible.height)
		{
			bulletArr.erase(i);
			i--;
			bullet->stopAllActions();
			bullet->monsterArr.clear();
			bullet->removeFromParentAndCleanup(true);
			continue;
		}

		if (!bullet->getAvaliable())
		{
			continue;
		}
		//将子弹和怪物的外围框转换，然后监测是否碰撞
		Point bulletPoint = bullet->convertToWorldSpace(Vec2(0, 0));
		Size bulletSize = bullet->getContentSize();
		Rect bulletRect(bulletPoint.x - bulletSize.width / 2, bulletPoint.y - bulletSize.height / 2,
			bulletSize.width, bulletSize.height);	//子弹的外围框
		for (int j = 0; j < monsters.size(); j++)
		{
			auto monster = monsters.at(j);
			Point monsterPoint = monster->convertToWorldSpace(Vec2(0, 0));
			Size monsterSize = monster->getContentSize();
			Rect monsterRect(monsterPoint.x - monsterSize.width / 2, monsterPoint.y - monsterSize.height / 2, 
				monsterSize.width, monsterSize.height);
			if (bulletRect.intersectsRect(monsterRect));
			{
				monster->setBlood(monster->getBlood() - getHarm());
				this->bulletArr.erase(i);
				i--;
				bullet->setAvaliable(false);
				bullet->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([=] {
					bullet->monsterArr.clear();
					bullet->removeFromParentAndCleanup(true);
				}), NULL));
				break;
			}
		}
	}
}

AbstractTower * AbstractTower::createTowerByString(std::string name)
{
	if (name == "Bottle.png")
	{
		return BottleTower::create();
	}
	if (name == "Fan.png")
	{
		return FanTower::create();
	}
	return nullptr;
}


AbstractTower* FanTower::create()
{
	auto tower = new FanTower();
	tower->setCanShoot(false);
	tower->setBulletInterval(0.8);
	tower->setRange(200);
	tower->setHarm(20);
	tower->setTowerName("Fan.png");
	tower->setBulletName("Fan1.png");
	tower->setTowerDirection(Point(0, 0));

	tower->initWithSpriteFrameName(tower->getTowerName());
	tower->setAnchorPoint(Vec2(0.5, 0.5));
	tower->autorelease();
	return tower;
}

void FanTower::update(float delta)
{
	this->setCanShoot(false);
	auto monsters = GameManager::getInstance()->monsters;
	for (int i = 0; i < monsters.size(); i++)
	{
		auto monster = monsters.at(i);
		if (monster->getBlood() > 0)
		{
			Point monsterPoint = monster->convertToWorldSpace(Vec2(0, 0));
			Point towerPoint = this->convertToWorldSpace(Vec2(0, 0));
			auto distance = monsterPoint.distance(towerPoint);
			if (distance <= this->getRange())
			{
				this->setCanShoot(true);
				break;
			}
		}
	}

	flashCount++;
	if (flashCount * Director::getInstance()->getAnimationInterval() > this->getBulletInterval() && canShoot)
	{
		flashCount = 0;
		shootBullet();
	}

	//检查子弹是否出界以及检查子弹和怪物是否碰撞 风扇的子弹会一直存在 直到出界
	//为了防止一个子弹和同一个怪物多次碰撞，每个子弹要记录当前和哪些怪物已经碰撞了
	
	for (int i = 0; i < bulletArr.size(); i++)
	{
		auto bullet = (FanBullet *)bulletArr.at(i);
		if (bullet->getPositionX() - origin.x < 0 || bullet->getPositionX() - origin.x > visible.width ||
			bullet->getPositionY() - origin.y < 0 || bullet->getPositionY() - origin.y > visible.height)
		{
			bulletArr.erase(i);
			i--;
			bullet->stopAllActions();
			bullet->monsterArr.clear();
			bullet->removeFromParentAndCleanup(true);
			continue;
		}

		Point bulletPoint = bullet->convertToWorldSpace(Vec2(0, 0));
		Size bulletSize = bullet->getContentSize();
		Rect bulletRect(bulletPoint.x - bulletSize.width / 2, bulletPoint.y - bulletSize.height / 2,
			bulletSize.width, bulletSize.height);
		for (int j = 0; j < monsters.size(); j++)
		{
			//跳过已经打过的怪物
			auto monster = monsters.at(j);
			bool skip = false;
			for (int k = 0; k < bullet->monsterArr.size(); k++)
			{
				if (monster == bullet->monsterArr.at(k))
				{
					skip = true;
					break;
				}
			}
			if (skip)
			{
				continue;
			}
			
			Point monsterPoint = monster->convertToWorldSpace(Vec2(0, 0));
			Size monsterSize = monster->getContentSize();
			Rect monsterRect(monsterPoint.x - monsterSize.width / 2, monsterPoint.y - monsterSize.height / 2,
				monsterSize.width, monsterSize.height);
			if (bulletRect.intersectsRect(monsterRect))
			{
				monster->setBlood(monster->getBlood() - getHarm());
				bullet->monsterArr.push_back(monster);
				break;
			}
		}
	}
	
}
void FanTower::shootBullet()
{
	auto bullet = FanBullet::create(this->getBulletName());
	bullet->setAnchorPoint(Vec2(0.5, 0.5));
	bullet->setPosition(this->getPositionX(), this->getPositionY() + this->getContentSize().height / 4);
	Vec2 direction = this->getTowerDirection() * 1000;
	bullet->runAction(MoveBy::create(bullet->getPosition().distance(direction) / bullet->getSpeed(), direction));
	this->bulletArr.pushBack(bullet);

	this->addChild(bullet);
}

AbstractBullet * FanBullet::create(std::string name)
{
	FanBullet* bullet = new FanBullet();
	bullet->setAvaliable(true);
	bullet->setSpeed(300);

	bullet->initWithSpriteFrameName(name);
	Vector<SpriteFrame *> vec;
	for (int i = 2; i <= 3; i++)
	{
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Fan" + std::to_string(i) + ".png");
		vec.pushBack(frame);
	}
	auto animation = Animation::createWithSpriteFrames(vec, 0.2f, -1);
	auto animate = Animate::create(animation);
	bullet->runAction(animate);
	bullet->autorelease();
	return bullet;
}

