#include "GameScene.h"

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event) {
	preX = touch->getLocation().x;
	if (!isStart) {
		isStart = true;

		//开始运动，用当前的触摸点决定初始运动方向

	}
	return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *unused_event) {
	if (isStart) {
		auto diff = touch->getLocation().x - preX;
		if (bottom->getPositionX() + diff < origin.x + bottom->getContentSize().width / 2) {
			bottom->setPosition(bottom->getContentSize().width / 2, bottom->getPositionY());
		}
		else if (bottom->getPositionX() + diff > origin.x + visible.width - bottom->getContentSize().width / 2) {
			bottom->setPosition(origin.x + visible.width - bottom->getContentSize().width / 2, bottom->getPositionY());
		}
		else {
			bottom->setPosition(bottom->getPositionX() + diff, bottom->getPositionY());
		}
	}
	preX = touch->getLocation().x;
}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event) {
}

void GameScene::initGame() {
	isStart = false;

	bottom->setVisible(true);
	bottom->setAnchorPoint(Vec2(0.5, 0));
	bottom->setPosition(origin.x + visible.width / 2, origin.y);
	ball->setVisible(true);
	ball->setAnchorPoint(Vec2(0.5, 0));
	ball->setPosition(origin.x + visible.width / 2, origin.y + bottom->getContentSize().height);

	for (int i = 0; i < bricks.size(); i++) {
		auto vec = bricks.at(i);
		for (int j = 0; j < vec.size(); j++) {
			auto brick = vec.at(j);
			brick->setAnchorPoint(Vec2(0, 0));
			brick->setPosition(origin.x + j * brick->getContentSize().width,
				origin.y + visible.height - i * brick->getContentSize().height);
			brick->setVisible(true);
		}
	}
}
bool GameScene::init() {
	if (!Layer::init())
	{
		return false;
	}
	visible = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	ball = Sprite::create("ball.png");
	bottom = Sprite::create("bottom.png");
	auto brick = Sprite::create("brick.png");

	ball->setVisible(false);
	bottom->setVisible(false);
	addChild(ball, 0);
	addChild(bottom, 0);

	int countX = visible.width / brick->getContentSize().width;
	int countY = visible.height * 0.4 / brick->getContentSize().height;

	for (int i = 0; i < countY; i++) {
		Vector<Sprite *> vec;
		for (int j = 0; j < countX; j++) {
			brick = Sprite::create("brick.png");
			brick->setVisible(false);
			vec.pushBack(brick);
			addChild(brick, 1);
		}
		bricks.push_back(vec);
	}
	initGame();

	auto listen = EventListenerTouchOneByOne::create();
	listen->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listen->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listen->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listen, this);
}

Scene* GameScene::createScene() {
	Scene* scene = Scene::create();
	auto gameScene = GameScene::create();

	scene->addChild(gameScene);
	return scene;
}