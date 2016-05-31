#include "Monster.h"
Monster *Monster::create(std::string name, float speed, float blood, Vector<Node *> nodes)
{
	auto monster = new Monster();
	monster->currentIndex = 0;
	monster->totalBlood = blood;
	monster->initWithSpriteFrameName(name);
	monster->setSpeed(speed);
	monster->setBlood(blood);
	monster->setIsDead(false);
	monster->setOpacity(255);
	monster->autorelease();
	monster->setAnchorPoint(Vec2(0.5, 0.5));
	monster->setCascadeOpacityEnabled(true);
	for (size_t i = 0; i < nodes.size(); i++)
	{
		monster->path.pushBack(nodes.at(i));
	}
	monster->setPosition(monster->path.at(0)->getPosition());

	monster->bloodBar = Sprite::createWithSpriteFrameName("xuetiao_empty.png");
	monster->bloodBar->setAnchorPoint(Vec2(0.5, 0));
	monster->bloodBar->setPosition(monster->getContentSize().width / 2, monster->getContentSize().height);
	monster->addChild(monster->bloodBar);

	monster->progressBar = ProgressTimer::create(Sprite::createWithSpriteFrameName("xuetiao_full.png"));
	monster->progressBar->setType(ProgressTimer::Type::BAR);
	monster->progressBar->setMidpoint(Vec2(0, 0.5));
	monster->progressBar->setBarChangeRate(Vec2(1, 0));
	monster->progressBar->setPercentage(100);
	monster->progressBar->setPosition(monster->bloodBar->getContentSize().width / 2, monster->bloodBar->getContentSize().height / 2);
	monster->bloodBar->addChild(monster->progressBar);

	
	return monster;
}

void Monster::forwad()
{
	currentIndex++;
	if (currentIndex < path.size())
	{
		direction = (path.at(currentIndex)->getPosition() - path.at(currentIndex - 1)->getPosition()).getNormalized();
		this->runAction(Sequence::create(MoveTo::create(getRunTime(), path.at(currentIndex)->getPosition()), 
			CallFunc::create(CC_CALLBACK_0(Monster::forwad, this)), NULL));
	}
	else {
		currentIndex = path.size();
		direction.x = 0;
		direction.y = 0;
	}
}
float Monster::getRunTime()
{
	Point p1 = path.at(currentIndex - 1)->getPosition();
	Point p2 = path.at(currentIndex)->getPosition();

	auto diff = p1.distance(p2);
	return diff / this->getSpeed();
}