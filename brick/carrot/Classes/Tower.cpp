#include "Tower.h"
AbstractTower::AbstractTower()
{
	Layer::init();
	this->autorelease();
}

AbstractTower* BottleTower::create()
{
	BottleTower* tower = new BottleTower();
	tower->BULLET_INTERVAL = 1;
	tower->BULLET_SPEED = 200;
	tower->setRange(100);
	tower->setHarm(25);
	tower->setBulletName("PBottle22.png");
	tower->autorelease();
	tower->scheduleUpdate();
	
	tower->towerBase = Sprite::create();
	tower->towerBase->initWithSpriteFrameName("Bottle23.png");
	tower->addChild(tower->towerBase);
	tower->scheduleUpdate();
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
	auto bullet = Sprite::createWithSpriteFrameName(this->getBulletName());
	auto degree = getNewRotation();
	auto rotation = CC_DEGREES_TO_RADIANS(-1 * degree);
	bullet->setRotation(degree);
	Vec2 direction;
	if (degree < -90)
	{
		direction.x = -1;
		direction.y = -tan(rotation);
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

	direction.x = direction.x * 10;
	direction.y = direction.y * 10;
	bullet->setPosition(towerBase->getPosition() + direction);

	direction.x = direction.x * 200;
	direction.y = direction.y * 200;
	bullet->setVisible(true);
	bullet->runAction(MoveTo::create(bullet->getPosition().distance(direction) / BULLET_SPEED, direction));
	bullets.pushBack(bullet);
	
	this->addChild(bullet, 3);
	log("x:%f y:%f x:%f y:%f", bullet->getPositionX(), bullet->getPositionY(), direction.x, direction.y);
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
	flashCount++;
	if (flashCount * Director::getInstance()->getAnimationInterval() > BULLET_INTERVAL)
	{
		flashCount = 0;
		shootBullet();
	}

	for (int i = 0; i < bullets.size(); i++) 
	{
		auto bullet = bullets.at(i);
		if (bullet->getPositionX() - origin.x < 0 || bullet->getPositionX() - origin.x > visible.width  ||
			bullet->getPositionY() - origin.y < 0 || bullet->getPositionY() - origin.y > visible.height)
		{
			bullets.erase(i);
			i--;
			bullet->stopAllActions();
			bullet->removeFromParentAndCleanup(true);
		}
	}
}