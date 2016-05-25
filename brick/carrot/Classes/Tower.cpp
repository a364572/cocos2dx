#include "Tower.h"
#include "GameManager.h"

AbstractTower* BottleTower::create()
{
	BottleTower* tower = new BottleTower();
	tower->BULLET_INTERVAL = 0.5;
	tower->BULLET_SPEED = 600;
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
	tower->autorelease();
	log("Frame Size width:%f height:%f", tower->towerBase->getContentSize().width, tower->towerBase->getContentSize().height);
	return tower;
}

void BottleTower::shootBullet() 
{
	//auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(this->getBulletName());
	//auto bullet = Sprite::createWithSpriteFrame(frame);
	auto bullet = Sprite::create(getBulletName());
	bullet->setAnchorPoint(Vec2(0.5, 0.5));
	//auto degree = towerBase->getRotation();
	//auto rotation = CC_DEGREES_TO_RADIANS(-1 * degree);
	bullet->setRotation(bullet_direction);

	Vec2 vec = getVectorOfTowerBase();
	vec.y = vec.y * 5;
	vec.x = vec.x * 5;
	bullet->setPosition(towerBase->getPosition() + vec);

	vec.x = vec.x * 400;
	vec.y = vec.y * 400;
	bullet->setOpacity(255);
	bullet->runAction(Sequence::create(MoveBy::create(bullet->getPosition().distance(vec) / BULLET_SPEED, vec),
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
				auto bulletDirection = monsterDirection * monster->getSpeed() + monsterToTower * BULLET_SPEED;

				auto degree = bulletDirection.getAngle();
				degree = CC_RADIANS_TO_DEGREES(-1 * degree);
				canShoot = true;

				//Vec2 vec = point - towerBase->getPosition();
				//auto degree = vec.getAngle();
				//degree = CC_RADIANS_TO_DEGREES(-1 * degree);
				//auto angle = Vec2::angle(vec, towerBaseVec);
				

				towerBase->setRotation(degree);
				this->bullet_direction = degree;
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

Point AbstractTower::getVectorOfTowerBase()
{
	auto degree = towerBase->getRotation();
	auto rotation = CC_DEGREES_TO_RADIANS(-1 * degree);
	Vec2 direction;
	if (degree < -90)
	{
		direction.x = -1;
		direction.y = tan(M_PI - rotation);
	}
	else if (degree == -90) {
		direction.x = 0;
		direction.y = 1;
	}
	else if (degree < 0) {
		direction.x = 1;
		direction.y = tan(rotation);
	}
	else if (degree == 0)
	{
		direction.x = 1;
		direction.y = 0;
	}
	else if (degree < 90)
	{
		direction.x = 1;
		direction.y = tan(rotation);
	}
	else if (degree == 90) {
		direction.x = 0;
		direction.y = -1;
	}
	else {
		direction.x = -1;
		direction.y = -tan(rotation);
	}
	return direction;
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
	tower->BULLET_INTERVAL = 0.8;
	tower->BULLET_SPEED = 200;
	tower->canShoot = false;
	tower->setRange(200);
	tower->setHarm(20);
	tower->setTowerName("Fan");
	tower->setBulletName("Fan-bullet");

	tower->towerBase = Sprite::create("FanTower.png");
	tower->towerBase->setAnchorPoint(Size(0.5, 0.5));
	Vector<SpriteFrame*> vec;
	for (int i = 1; i <= 3; i++)
	{
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(tower->getBulletName() + std::to_string(i) + ".png");
		vec.pushBack(frame);
	}
	auto animation = Animation::createWithSpriteFrames(vec, 0.3f, -1);
	auto animate = Animate::create(animation);
	tower->towerBase->runAction(animate);

	//风车的底盘
	auto sprite = Sprite::create("Fan-bottom.png");
	sprite->setAnchorPoint(Vec2(0.5, 0));
	tower->setContentSize(Size(tower->towerBase->getContentSize().width, 
		sprite->getContentSize().height + tower->towerBase->getContentSize().height));
	sprite->setPosition(tower->getContentSize().width / 2, 0);
	tower->addChild(sprite);

	//风车叶子
	tower->towerBase->setAnchorPoint(Vec2(0.5, 0));
	tower->towerBase->setPosition(tower->getContentSize().width / 2, sprite->getContentSize().height);
	tower->addChild(tower->towerBase);

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
			Point point = convertToNodeSpace(monster->convertToWorldSpace(Vec2(0, 0)));
			auto distance = point.distance(towerBase->getPosition());

			if (distance <= getRange())
			{
				auto monsterToTower = (point - towerBase->getPosition()).getNormalized();
				auto monsterDirection = (monster->direction).getNormalized();
				auto bulletDirection = monsterDirection * monster->getSpeed() + monsterToTower * BULLET_SPEED;

				auto degree = bulletDirection.getAngle();
				degree = CC_RADIANS_TO_DEGREES(-1 * degree);
				canShoot = true;
				bullet_direction = degree;
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

	for (int i = 0; i < bullets.size(); i++)
	{
		auto bullet = bullets.at(i);
		if (bullet->getPositionX() - origin.x < 0 || bullet->getPositionX() - origin.x > visible.width ||
			bullet->getPositionY() - origin.y < 0 || bullet->getPositionY() - origin.y > visible.height)
		{
			bullets.erase(i);
			i--;
			bullet->stopAllActions();
			bullet->removeFromParentAndCleanup(true);
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
			if (rect1.intersectsRect(rect2))
			{
				monster->setBlood(monster->getBlood() - getHarm());
				break;
			}
		}
	}
}
void FanTower::shootBullet()
{

}