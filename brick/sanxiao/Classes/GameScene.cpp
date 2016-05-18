#include "GameScene.h"

Scene* GameScene::createScene() {
	auto scene = Scene::create();
	auto gameScene = GameScene::create();
	scene->addChild(gameScene);
	log("createScene");
	return scene;
}

bool GameScene::init() {

	if (!Layer::init()) {
		return false;
	}
	srand(time(0));

	visible = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("bg.png");
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(origin.x, origin.y);
	addChild(bg, -1);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("head.plist", "head.png");

	log("visibleX:%f visibleY:%f originX:%f origin:%f", visible.width, visible.height, origin.x, origin.y);
	initFruit();
	return true;
}

void GameScene::onEnterTransitionDidFinish()
{
	download();
	judgeAll();
}

void GameScene::initFruit() {

	auto demo = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(FruitSprite::nameArray[0]));
	int countX = visible.width / demo->getContentSize().width;
	int countY = visible.height * 0.8 / demo->getContentSize().height;
	
	for (int i = 0; i < countY; i++) {
		Vector<FruitSprite *> vec;
		for (int j = 0; j < countX; j++)
		{
			auto fruit = FruitSprite::create(i, j, -1);
			fruit->setAnchorPoint(Vec2(0, 0));
			fruit->setPosition(origin.x + j * demo->getContentSize().width,
				origin.y + visible.height + i * demo->getContentSize().height);
			addChild(fruit, 1);
			vec.pushBack(fruit);
		}
		fruits.push_back(vec);
	}
	
}

void GameScene::download() {
	for (int i = 0; i < fruits.size(); i++)
	{
		auto vec = fruits.at(i);
		for (int j = 0; j < vec.size(); j++) {
			auto fruit = vec.at(j);
			fruit->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(0, 50 - visible.height)),
				[=] {
					if (i == fruits.size() && j == vec.size()) {
						judgeAll();
					}
				}, NULL));
		}
	}
}

bool GameScene::judgeAll()
{
	GameLine line;
	for (int i = 0; i < fruits.size(); i++) {
		for (int j = 0; j < fruits[i].size(); j++) {
			//首先判断是否有合适的
			if (judgeInPosition(i, j, &line)) {
				int index = 0;
				switch (line.direction)
				{
				case 1:

					for (int k = i ; k < fruits.size() - line.length; k++)
					{
						Point pos = fruits[k].at(j)->getPosition();
						auto newFruit = FruitSprite::create(k, j, fruits[k + line.length].at(j)->getType());
						newFruit->setAnchorPoint(Vec2(0, 0));
						newFruit->setPosition(fruits[k + line.length].at(j)->getPosition());
						fruits[k].at(j)->removeFromParent();
						fruits[k].replace(j, newFruit);
						addChild(newFruit, 1);
						newFruit->runAction(MoveBy::create(1.5f, Vec2(0, -line.length * newFruit->getContentSize().height)));
					}
					for (int k = fruits.size() - line.length; k < fruits.size();  k++) {
						Point pos = fruits[k].at(j)->getPosition();
						auto newFruit = FruitSprite::create(k, j, -1);
						newFruit->setAnchorPoint(Vec2(0, 0));
						newFruit->setPosition(pos.x, pos.y + newFruit->getContentSize().height * line.length);
						fruits[k].at(j)->removeFromParent();
						fruits[k].replace(j, newFruit);
						addChild(newFruit, 1);
						fruits[k].at(j)->runAction(MoveTo::create(0.5f, pos));
					}
					log("x:%d y:%d type:%d len:%d", i, j, line.direction, line.length);
					j--;
					break;
				case 4:
					break;
				default:
					break;
				}
				
			}
		}
	}
	return false;
}

bool GameScene::judgeInPosition(int row, int col, GameLine *line)
{
	int count = 1;
	int threshold = 3;
	int i = row + 1;
	int j = col;
	int type = fruits[row].at(col)->getType();
	//因为是从下往上从左到右扫描，只需考虑向上和向右的
	//top
	while (i < fruits.size() && fruits[i].at(j)->getType() == type) {
		i++;
		count++;
	}
	if (count >= threshold) {
		line->direction = 1;
		line->length = count;
		return true;
	}
	/*
	//bottom
	i = row - 1;
	j = col;
	count = 1;
	while (i >= 0 && fruits[i].at(j)->getType() == type)
	{
		i--;
		count++;
	}
	if (count >= threshold) {
		return true;
	}
	//left
	i = row;
	j = col - 1;
	count = 1;
	while (j >= 0 && fruits[i].at(j)->getType() == type)
	{
		j--;
		count++;
	}
	if (count >= threshold)	{
		return true;
	}
	*/
	//right
	i = row;
	j = col + 1;
	count = 1;
	while (j < fruits[i].size() && fruits[i].at(j)->getType() == type) {
		j++; 
		count++;
	}
	if (count >= threshold) {
		line->direction = 4;
		line->length = count;
		return true;
	}
	return false;
}
