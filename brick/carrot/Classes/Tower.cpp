#include "Tower.h"
#include "GameManager.h"

AbstractTower* BottleTower::create()
{
	BottleTower* tower = new BottleTower();
	tower->BULLET_INTERVAL = 3;
	tower->BULLET_SPEED = 100;
	tower->setRange(100);
	tower->setHarm(100);
	tower->setTowerName("Bottle");
	tower->setBulletName("PBottle11.png");
	
	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName("Bottle23.png");
	tower->towerBase = Sprite::createWithSpriteFrame(frame);
	tower->towerBase->setAnchorPoint(Vec2(0.5, 0.5));
	tower->setContentSize(frame->getRect().size);
	tower->addChild(tower->towerBase);
	tower->autorelease();
	return tower;
}

float BottleTower::getNewRotation() {
	float degree = this->getRotation() - 90;
	if (degree > 90)
	{
		degree = degree - 90;
	}
	if (degree < -180)
	{
		degree = 360 + degree;
	}
	return degree;

}
void BottleTower::shootBullet() 
{
	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(this->getBulletName());
	auto bullet = Sprite::createWithSpriteFrame(frame);
	bullet->setContentSize(frame->getRect().size);
	auto degree = getNewRotation();
	auto rotation = CC_DEGREES_TO_RADIANS(-1 * degree);
	bullet->setRotation(degree);
	Vec2 direction;
	if (degree < -90)
	{
		direction.x = -1;
		direction.y = tan(M_PI - rotation);
		//rotation = M_PI - rotation;
		//direction.x = -getRange() * cos(rotation);
		//direction.y = getRange() * sin(rotation);
	}
	else if (degree == -90) {
		direction.x = 0;
		direction.y = 1;
		//rotation = M_PI / 2;
		//direction.x = 0;
		//direction.y = getRange();
	}
	else if (degree < 0) {
		direction.x = 1;
		direction.y = tan(rotation);
		//direction.x = getRange() * cos(rotation);
		//direction.y = getRange() * sin(rotation);
	}
	else if (degree == 0)
	{
		direction.x = 1;
		direction.y = 0;
		//rotation = 0;
		//direction.x = getRange();
		//direction.y = 0;
	}
	else if (degree < 90) 
	{
		direction.x = 1;
		direction.y = tan(rotation);
		//rotation = -rotation;
		//direction.x = getRange() * cos(rotation);
		//direction.y = -getRange() * sin(rotation);
	}
	else if (degree == 90) {
		direction.x = 0;
		direction.y = -1;
		//rotation = -90;
		//direction.x = 0;
		//direction.y = -getRange();
	}
	else {
		direction.x = -1;
		direction.y = -tan(rotation);
		//rotation = M_PI + rotation;
		//direction.x = getRange() * cos(rotation);
		//direction.y = -getRange() * sin(rotation);
	}

	Point vec = direction;
	vec.y = vec.y * 5;
	vec.x = vec.x * 5;
	bullet->setPosition(towerBase->getPosition() + vec);

	vec.x = vec.x * 400;
	vec.y = vec.y * 400;
	bullet->setOpacity(255);
	bullet->runAction(Sequence::create(MoveTo::create(bullet->getPosition().distance(vec) / BULLET_SPEED, vec),
		CallFunc::create([=] {bullet->setOpacity(0); }),
		NULL));
	bullets.pushBack(bullet);
	
	this->addChild(bullet);
	log("shoot x:%f y:%f x:%f y:%f", bullet->getPositionX(), bullet->getPositionY(), direction.x, direction.y);
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

	//首先检查是否要发射子弹
	flashCount++;
	if (flashCount * Director::getInstance()->getAnimationInterval() > BULLET_INTERVAL)
	{
		flashCount = 0;
		shootBullet();
	}
	auto monsters = GameManager::getInstance()->monsters;
	for (int i = 0; i < monsters.size(); i++)
	{
		auto monster = monsters.at(i);
		if (monster->getOpacity() == 255)
		{
			//Point point = towerBase->convertToNodeSpace()
		}
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
				log("x:%f y:%f width:%f height:%f x:%f y:%f width:%f height:%f", rect1.getMinX(), rect1.getMinY(), rect1.getMaxX(), rect1.getMaxY(), 
					rect2.getMinX(), rect2.getMinY(), rect2.getMaxX(), rect2.getMaxY());
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
	return nullptr;
}
