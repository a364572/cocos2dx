#include "FruitSprite.h"

std::vector<std::string> FruitSprite::nameArray = {"h0", "h1", "h2", "h3", "h4"};


FruitSprite* FruitSprite::create(int row, int col, int type) {
	auto visible = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto sprite = new FruitSprite();
	sprite->setRow(row);
	sprite->setCol(col);
	if (type < 0 || type >= nameArray.size()) {
		type = rand() % (nameArray.size());
	} 
	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(nameArray[type]);
	sprite->setType(type);
	sprite->initWithSpriteFrame(frame);
	sprite->setLegal(true);
	sprite->setAnchorPoint(Vec2(0, 0));
	sprite->autorelease();

	int count = visible.width / sprite->getContentSize().width;
	auto rate = visible.width / count / sprite->getContentSize().width;
	sprite->setScale(rate);
	sprite->setContentSize(Size(visible.width / count, sprite->getContentSize().height * rate));
	return sprite;
}