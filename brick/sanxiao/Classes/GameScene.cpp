#include "GameScene.h"

#define BOTTOM_INTERVAL 50
#define DOWN_SPEED_PER_SECOND 200
Scene* GameScene::createScene() {
	auto scene = Scene::create();
	auto gameScene = GameScene::create();
	scene->addChild(gameScene);
	return scene;
}

bool GameScene::init() {

	if (!Layer::init()) {
		return false;
	}
	srand(time(0));

	visible = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	reference = 0;

	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto bg = Sprite::create("bg.png");
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(origin.x, origin.y);
	addChild(bg, -1);

	TTFConfig ttf("fonts/Marker Felt.ttf", 30);
	auto label = Label::createWithTTF(ttf, "Restart", TextHAlignment::CENTER);
	auto menuRestart = MenuItemLabel::create(label, CC_CALLBACK_1(GameScene::restartGame, this));
	auto menu = Menu::create(menuRestart, NULL);
	menu->alignItemsVertically();
	menu->setAnchorPoint(Vec2(0, 1));
	menu->setPosition(origin.x + 50, origin.y + visible.height - 20);
	addChild(menu, 2);

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("head.plist", "head.png");

	log("visibleX:%f visibleY:%f originX:%f origin:%f", visible.width, visible.height, origin.x, origin.y);
	initFruit();
	return true;
}

void GameScene::onEnterTransitionDidFinish()
{
	download();
	//judgeAll();
}

void GameScene::restartGame(Ref * ref)
{
	initFruit();
	reference = 0;
	download();
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event) {
	if (reference != 0 || isDown)
	{
		log("Ref:%d, isDown:%d", reference, isDown);
		return false;
	}
	preFruit = NULL;
	preFruit = getSpriteFromPosition(touch->getLocation());
	if (preFruit)
	{
		return true;
	}
	return false;
}
void GameScene::onTouchMoved(Touch *touch, Event *unused_event) {
	
}
void GameScene::onTouchEnded(Touch *touch, Event *unused_event) {
	auto current = getSpriteFromPosition(touch->getLocation());
	if (reference == 0 && current && preFruit && current != preFruit) {
		if (abs(current->getRow() - preFruit->getRow()) + abs(current->getCol() - preFruit->getCol()) == 1)
		{
			reference += 2;
			auto time = swapPosition(preFruit, current);
			auto pos1 = preFruit->getPosition();
			auto pos2 = current->getPosition();
			current->runAction(Sequence::create(MoveTo::create(time, pos1), CallFunc::create([=]
			{
				reference--; 
				if (reference == 0)
				{
					if (!judgeAll())
					{
						isDown = true;
						swapPosition(preFruit, current);
						current->runAction(MoveTo::create(time, pos2));
						preFruit->runAction(MoveTo::create(time, pos1));
						isDown = false;
					}
				}
			}), NULL));
			preFruit->runAction(Sequence::create(MoveTo::create(time, pos2), CallFunc::create([=]
			{
				reference--;
				if (reference == 0)
				{
					if (!judgeAll())
					{
						isDown = true;
						swapPosition(preFruit, current);
						current->runAction(MoveTo::create(time, pos2));
						preFruit->runAction(MoveTo::create(time, pos1));
						isDown = false;
					}
				}
			}), NULL));
		}
	}
}

FruitSprite * GameScene::getSpriteFromPosition(Point point)
{
	if (fruits.size() == 0)
	{
		return NULL;
	}
	auto sprite = fruits.front().front();
	int row = (point.y - BOTTOM_INTERVAL - origin.y) / sprite->getContentSize().height;
	int col = (point.x - origin.x) / sprite->getContentSize().width;
	if (row < 0 || row >= fruits.size() || col < 0 || col >= fruits.front().size())
	{
		return NULL;
	}
	return fruits[row].at(col);
}

void GameScene::initFruit() {
	reference++;
	auto demo = FruitSprite::create(0, 0, 0);
	int countX = visible.width / demo->getContentSize().width;
	int countY = visible.height * 0.8 / demo->getContentSize().height;

	isDown = true;
	if (fruits.size() > 0) {
		for (int i = 0; i < countY; i++)
		{
			for (int j = 0; j < countX; j++)
			{
				auto fruit = fruits[i].at(j);
				fruit->stopAllActions();
				int index = rand() % FruitSprite::nameArray.size();
				fruit->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(FruitSprite::nameArray[index]));
				fruit->setType(index);
				fruit->setLegal(true);
				fruit->setOpacity(255);
				fruit->setPosition(origin.x + j * demo->getContentSize().width,
					origin.y + visible.height + i * demo->getContentSize().height);
			}
		}
	}
	else {

		for (int i = 0; i < countY; i++) {
			Vector<FruitSprite *> vec;
			for (int j = 0; j < countX; j++)
			{
				auto fruit = FruitSprite::create(i, j, -1);
				fruit->setPosition(origin.x + j * demo->getContentSize().width,
					origin.y + visible.height + i * demo->getContentSize().height);
				addChild(fruit, 1);
				vec.pushBack(fruit);
			}
			fruits.push_back(vec);
		}
	}
	reference--;
}

void GameScene::download() {
	
	for (int i = 0; i < fruits.size(); i++)
	{
		auto vec = fruits.at(i);
		for (int j = 0; j < vec.size(); j++) {
			auto fruit = vec.at(j);
			auto diff = BOTTOM_INTERVAL - visible.height;
			auto time = abs(diff) / DOWN_SPEED_PER_SECOND;
			fruit->runAction(Sequence::create(MoveBy::create(time, Vec2(0, diff)), CallFunc::create([=] {
				if (fruit->getRow() == fruits.size() - 1 && fruit->getCol() == fruits.back().size() - 1) {
					judgeAll();
				}
			}),
				NULL));
		}
	}
}

float GameScene::swapPosition(FruitSprite *sp1, FruitSprite *sp2)
{
	//首先交换数组中的位置，然后交换行和列
	fruits[sp1->getRow()].replace(sp1->getCol(), sp2);
	fruits[sp2->getRow()].replace(sp2->getCol(), sp1);

	auto row = sp1->getRow();
	auto col = sp1->getCol();
	auto time = abs(sp2->getPositionY() - sp1->getPositionY()) / DOWN_SPEED_PER_SECOND;
	if (sp1->getRow() == sp2->getRow())
	{
		time = abs(sp2->getPositionX() - sp1->getPositionX()) / DOWN_SPEED_PER_SECOND;
	}
	sp1->setRow(sp2->getRow());
	sp1->setCol(sp2->getCol());
	sp2->setRow(row);
	sp2->setCol(col);
	return time;
}

/************************************************************************/
/* 先遍历所有找出能够消除的，然后再依次填充                                                                     */
/************************************************************************/
bool GameScene::judgeAll()
{
	reference++;
	isDown = true;
	bool flag = false;
	int row = fruits.size();
	int col = fruits.front().size();
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			int res = judgeInPosition(i, j);
			if (res > 0) {
				flag = true;
			}
		}
	}

	if (!flag)
	{
		reference--;
		isDown = false;
		return false;
	}
	//每一列往下沉 downPos保存每一列中没有消失的节点
	vector<vector<int>> downPos;
	Vector<FruitSprite *> fadeSprites;
	for (int i = 0; i < col; i++) {
		vector<int> vec;
		for (int j = 0; j < row; j++)
		{
			if (fruits[j].at(i)->getLegal()) {
				vec.push_back(j);
			}
			else {
				fadeSprites.pushBack(fruits[j].at(i));
			}
		}
		downPos.push_back(vec);
	}

	flashCount = 0;
	int fadeCount = fadeSprites.size();
	//然后让每个节点往下掉
	for (int i = 0; i < fadeCount; i++) {
		fadeSprites.at(i)->runAction(Sequence::create(FadeOut::create(1.0f), CallFunc::create([=] {

			if (i == fadeCount - 1) {
				//最后一个 重新加载元素
				for (int j = 0; j < col; j++) {

					for (int k = 0; k < downPos[j].size(); k++) {
						int index = downPos[j][k];
						if (index == k)	//元素不变，跳过
						{
							continue;
						}
						auto newFruit = FruitSprite::create(k, j, fruits[index].at(j)->getType());
						auto oldFruit = fruits[k].at(j);
						auto diff = (k - index) * newFruit->getContentSize().height;
						newFruit->setPosition(fruits[index].at(j)->getPosition());
						fruits[k].replace(j, newFruit);
						addChild(newFruit, 1);
						oldFruit->removeFromParentAndCleanup(true);
						newFruit->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(0, diff)), NULL));
						
					}

					
					for (int k = downPos[j].size(); k < row; k++) {
						auto newFruit = FruitSprite::create(k, j, -1);
						auto oldFruit = fruits[k].at(j);
						newFruit->setPosition(origin.x + j * newFruit->getContentSize().width,
							origin.y + BOTTOM_INTERVAL + (row + k - downPos[j].size()) * newFruit->getContentSize().height);
						fruits[k].replace(j, newFruit);
						addChild(newFruit, 1);
						oldFruit->removeFromParentAndCleanup(true);
						int diff = downPos[j].size() - row;
						newFruit->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(0, newFruit->getContentSize().height * diff)), CallFunc::create([=] {
							flashCount++;
							if (flashCount >= fadeCount)
							{
								reference--;
								GameScene::judgeAll();
							}
						}), NULL));
					}
					
				}
			}
		}), NULL));
	}
	return flag;
}

/************************************************************************/
/*
Line.direction:
	0x1		: top
	0x10	: bottom
	0x100	: left
	0x1000	: right
*/
/************************************************************************/
int GameScene::judgeInPosition(int row, int col)
{
	int count = 1;
	int threshold = 3;
	int i = row + 1;
	int j = col;
	int type = fruits[row].at(col)->getType();
	int flag = 0;
	//因为是从下往上从左到右扫描，只需考虑向上和向右的
	//top
	while (i < fruits.size() && fruits[i].at(j)->getType() == type) {
		i++;
		count++;
	}
	if (count >= threshold) {
		for (i = 0; i < count; i++) {
			fruits[i + row].at(col)->setLegal(false);
		}
		flag |= 0x1;
	}

	i = row;
	j = col + 1;
	count = 1;
	while (j < fruits[i].size() && fruits[i].at(j)->getType() == type) {
		j++; 
		count++;
	}
	if (count >= threshold) {
		for (i = 0; i < count; i++) {
			fruits[row].at(col + i)->setLegal(false);
		}
		flag |= 0x1000;
	}
	return flag;
}
