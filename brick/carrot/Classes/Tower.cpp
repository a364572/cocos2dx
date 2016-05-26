#include "Tower.h"
#include "GameManager.h"

AbstractTower* BottleTower::create()
{
	BottleTower* tower = new BottleTower();
	tower->setColor(Color3B(199, 255, 34));
	tower->BULLET_INTERVAL = 1;
	tower->BULLET_SPEED = 200;
	tower->canShoot = false;
	tower->setRange(200);
	tower->setHarm(20);
	tower->setTowerName("Bottle");
	tower->setBulletName("Bottle-bullet.png");
	
	//auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Bottle11.png");
	//tower->towerBase = Sprite::createWithSpriteFrame(frame);
	tower->towerBase = Sprite::create("Bottle.png");
	tower->towerBase->setAnchorPoint(Size(0.5, 0.5));
	tower->setContentSize(tower->towerBase->getContentSize());
	tower->addChild(tower->towerBase);
	//tower->ignoreAnchorPointForPosition(false);
	tower->setAnchorPoint(Vec2(0.5, 0.5));
	tower->autorelease(); 
	return tower;
}

void BottleTower::shootBullet() 
{
	auto bullet = Sprite::create(getBulletName());
	bullet->setAnchorPoint(Vec2(0.5, 0.5));
	bullet->setPosition(towerBase->getPosition() + bulletDirection * 5);
	
	bulletDirection = bulletDirection * 1000;
	bullet->setOpacity(255);
	bullet->runAction(Sequence::create(MoveBy::create(bullet->getPosition().distance(bulletDirection) / BULLET_SPEED, bulletDirection),
		CallFunc::create([=] {bullet->setOpacity(0); }),
		NULL));
	bullets.pushBack(bullet);
	
	this->addChild(bullet);
}

float BottleTower::getRotation()
{
	return towerBase->getRotation();
}

void BottleTower::setRotation(float rotation)
{
	towerBase->setRotation(rotation);
}

void BottleTower::update(float dt) {

	//检查炮台的攻击范围内是否有怪物 有的话先旋转角度后射击
	canShoot = false;
	auto monsters = GameManager::getInstance()->monsters;
	for (int i = 0; i < monsters.size(); i++)
	{
		auto monster = monsters.at(i);
		if (monster->getBlood() > 0)
		{
			Point point = convertToNodeSpace(monster->convertToWorldSpace(Vec2(0, 0)));
			auto distance = point.distance(towerBase->getPosition());
			
			if (distance <= getRange())
			{
				auto monsterToTower = (point - towerBase->getPosition()).getNormalized();
				auto monsterDirection = (monster->direction).getNormalized();
				bulletDirection = (monsterDirection * monster->getSpeed() + monsterToTower * BULLET_SPEED).getNormalized();

				auto degree = bulletDirection.getAngle();
				degree = CC_RADIANS_TO_DEGREES(-1 * degree);
				canShoot = true;

				//Vec2 vec = point - towerBase->getPosition();
				//auto degree = vec.getAngle();
				//degree = CC_RADIANS_TO_DEGREES(-1 * degree);
				//auto angle = Vec2::angle(vec, towerBaseVec);
				towerBase->setRotation(degree);
				/*
				if (angle < M_PI / 4)
				{
					towerBase->setRotation(degree);
				}
				else if (towerBase->getNumberOfRunningActions() == 0)
				{
					log("monster %d angle:%f", i, angle);
					towerBase->runAction(Sequence::create(RotateTo::create(0.5f / (angle / M_PI), degree), CallFunc::create([=] {canShoot = true; }), NULL));
				}
				*/
				break;
			}
		}
	}

	//检查是否要发射子弹
	flashCount++;
	if (flashCount * Director::getInstance()->getAnimationInterval() > BULLET_INTERVAL && canShoot)
	{
		flashCount = 0;
		shootBullet();
	}

	//检查子弹是否出界以及检查子弹和怪物是否碰撞
	
	for (int i = 0; i < bullets.size(); i++) 
	{
		auto bullet = bullets.at(i);
		if (bullet->getOpacity() == 0 || 
			bullet->getPositionX() - origin.x < 0 || bullet->getPositionX() - origin.x > visible.width  ||
			bullet->getPositionY() - origin.y < 0 || bullet->getPositionY() - origin.y > visible.height)
		{
			//log("x:%f y:%f", bullet->getPositionX() - origin.x, bullet->getPositionY() - origin.y);
			bullets.erase(i);
			i--;
			bullet->stopAllActions();
			bullet->removeFromParentAndCleanup(true);
			continue;
		}

		if (bullet->getOpacity() < 255)
		{
			continue;
		}
		Point point = bullet->convertToWorldSpace(Vec2(0, 0));
		Size size = bullet->getContentSize();
		Rect rect1(point.x - size.width / 2, point.y - size.height / 2, size.width, size.height);
		for (int j = 0; j < monsters.size(); j++)
		{
			auto monster = monsters.at(j);
			Point point1 = monster->convertToWorldSpace(Vec2(0, 0));
			Size size1 = monster->getContentSize();
			Rect rect2(point1.x - size1.width / 2, point1.y - size1.height / 2, size1.width, size1.height);
			if (bullet->getOpacity() == 255 && rect1.intersectsRect(rect2));
			{
				if (rect1.getMaxX() < rect2.getMinX())
				{
					continue;
				}
				if (rect2.getMaxX() < rect1.getMinX())
				{
					continue;
				}
				if (rect2.getMaxY() < rect1.getMinY())
				{
					continue;
				}
				if (rect1.getMaxY() < rect2.getMinY())
				{
					continue;
				}
				//log("x:%f y:%f width:%f height:%f x:%f y:%f width:%f height:%f", rect1.getMinX(), rect1.getMinY(), rect1.getMaxX(), rect1.getMaxY(), 
				//	rect2.getMinX(), rect2.getMinY(), rect2.getMaxX(), rect2.getMaxY());
				bullet->setOpacity(254);
				bullet->runAction(FadeOut::create(0.5f));
				monster->setBlood(monster->getBlood() - getHarm());
				break;
			}
		}
	}
}

AbstractTower * AbstractTower::createTowerByString(std::string name)
{
	if (name == "Bottle")
	{
		return BottleTower::create();
	}
	if (name == "Fan")
	{
		return FanTower::create();
	}
	return nullptr;
}


AbstractTower* FanTower::create()
{
	auto tower = new FanTower();
	tower->setColor(Color3B(255, 255, 255));
	tower->BULLET_INTERVAL = 0.8;
	tower->BULLET_SPEED = 200;
	tower->canShoot = false;
	tower->setRange(200);
	tower->setHarm(20);
	tower->setTowerName("Fan");
	tower->setBulletName("Fan-bullet.png");

	//风车的底盘
	tower->fanBottom = Sprite::create("Fan-bottom.png");
	tower->fanBottom->setAnchorPoint(Vec2(0.5, 0));
	tower->setPositionY(0);
	tower->addChild(tower->fanBottom);

	//风车叶子	
	tower->towerBase = Sprite::create("FanTower.png");
	tower->towerBase->setAnchorPoint(Vec2(0.5, 0.5));
	tower->towerBase->setPosition(tower->towerBase->getContentSize().width / 2, tower->fanBottom->getContentSize().height);
	tower->addChild(tower->towerBase);
	tower->fanBottom->setPositionX(tower->towerBase->getContentSize().width / 2);

	tower->setContentSize(Size(tower->towerBase->getContentSize().width,
		tower->fanBottom->getContentSize().height + tower->towerBase->getContentSize().height / 2));
	tower->ignoreAnchorPointForPosition(false);
	tower->setAnchorPoint(Vec2(0.5, 0.5));
	tower->autorelease();

	return tower;
}

void FanTower::update(float delta)
{
	//检查炮台的攻击范围内是否有怪物 有的话可以开始射击
	canShoot = false;
	auto monsters = GameManager::getInstance()->monsters;
	for (int i = 0; i < monsters.size(); i++)
	{
		auto monster = monsters.at(i);
		if (monster->getBlood() > 0)
		{
			//Point point = convertToNodeSpace(monster->convertToWorldSpace(Vec2(0, 0)));
			//auto distance = point.distance(towerBase->getPosition());
			Point monsterPoint = monster->convertToWorldSpace(Vec2(0, 0));
			Point towerPoint = towerBase->convertToWorldSpace(Vec2(0, 0));
			auto distance = monsterPoint.distance(towerPoint);
			if (distance <= getRange())
			{
				//auto monsterToTower = (point - towerBase->getPosition()).getNormalized();
				auto monsterToTower = (monsterPoint - towerPoint).getNormalized();
				auto monsterDirection = (monster->direction).getNormalized();
				bulletDirection = (monsterDirection * monster->getSpeed() + monsterToTower * BULLET_SPEED).getNormalized();
				canShoot = true;
				break;
			}
		}
	}

	//检查是否要发射子弹
	flashCount++;
	if (flashCount * Director::getInstance()->getAnimationInterval() > BULLET_INTERVAL && canShoot)
	{
		flashCount = 0;
		shootBullet();
	}

	//检查子弹是否出界以及检查子弹和怪物是否碰撞 风扇的子弹会一直存在 直到出界
	//为了防止一个子弹和同一个怪物多次碰撞，每个子弹要记录当前和哪些怪物已经碰撞了
	
	for (int i = 0; i < bullets.size(); i++)
	{
		auto bullet = (FanBullet *)bullets.at(i);
		if (bullet->getPositionX() - origin.x < 0 || bullet->getPositionX() - origin.x > visible.width ||
			bullet->getPositionY() - origin.y < 0 || bullet->getPositionY() - origin.y > visible.height)
		{
			bullets.erase(i);
			i--;
			bullet->stopAllActions();
			bullet->monsterArr.clear();
			bullet->removeFromParentAndCleanup(true);
			continue;
		}

		Point point = bullet->convertToWorldSpace(Vec2(0, 0));
		Size size = bullet->getContentSize();
		Rect rect1(point.x - size.width / 2, point.y - size.height / 2, size.width, size.height);
		for (int j = 0; j < monsters.size(); j++)
		{
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
			
			Point point1 = monster->convertToWorldSpace(Vec2(0, 0));
			Size size1 = monster->getContentSize();
			Rect rect2(point1.x - size1.width / 2, point1.y - size1.height / 2, size1.width, size1.height);
			if (rect1.intersectsRect(rect2))
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
	auto bullet = FanBullet::create();
	bullet->setAnchorPoint(Vec2(0.5, 0.5));
	bullet->setPosition(towerBase->getPosition());
	bulletDirection = bulletDirection * 1000;
	bullet->runAction(MoveBy::create(bullet->getPosition().distance(bulletDirection) / BULLET_SPEED, bulletDirection));
	bullets.pushBack(bullet);

	this->addChild(bullet);
}

FanBullet * FanBullet::create()
{
	FanBullet* bullet = new FanBullet();
	bullet->initWithFile("Fan-bullet.png");
	bullet->setBulletName("Fan");
	Vector<SpriteFrame *> vec;
	for (int i = 1; i <= 3; i++)
	{
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(bullet->getBulletName() + std::to_string(i) + ".png");
		vec.pushBack(frame);
	}
	auto animation = Animation::createWithSpriteFrames(vec, 0.2f, -1);
	auto animate = Animate::create(animation);
	bullet->runAction(animate);
	bullet->autorelease();
	return bullet;
}
