#include "GameScene.h"

#define ACC_SPEED 10
bool GameScene::onTouchBegan(Touch *touch, Event *unused_event) {
	preX = touch->getLocation().x;
	preFlashCount = curFlashCount = 0;
	horAcceration = 0;
	if (!isStart) {
		isStart = true;
		scheduleUpdate();

		//开始运动，用当前的触摸点决定初始运动方向
		Vec2 vec = touch->getLocation() - ball->convertToWorldSpace(Vec2(0, 0));

		auto angle = vec.getAngle();
		auto degree = CC_RADIANS_TO_DEGREES(-1 * angle);
		log("Radians:%f Degree:%f", angle, degree);

		float x, y;
		if (degree < 0 && degree > -180) {
			if (degree < -90) {
				angle = M_PI - angle;
				x = -1;
				y = tan(angle);				
			}
			else {
				x = 1;
				y = tan(angle);
			}
		}
		else {
			x = 0;
			y = 1;
		}

		preDirection.x = x;
		preDirection.y = y;
		moveBall(preDirection);

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
		if (curFlashCount - preFlashCount > 0) {
			float flashTime = Director::getInstance()->getAnimationInterval() * (curFlashCount - preFlashCount);
			horAcceration = diff / flashTime;
		}

		preFlashCount = curFlashCount;
		
	}
	preX = touch->getLocation().x;
}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event) {
	preFlashCount = curFlashCount = 0;
	horAcceration = 0;
}

void GameScene::collisionTop() {
	auto angle = preDirection.getAngle();
	auto degree = CC_RADIANS_TO_DEGREES(-1 * angle);
	log("top    Degree:%f Angle:%f", degree, angle);
	//if ((ball->getPositionY() >= origin.y + visible.height) && preDirection.y > 0) {
		if (degree > -90) {
			preDirection.x = 1;
			preDirection.y = -tan(angle);
		}
		else if (degree < -90) {
			preDirection.x = -1;
			preDirection.y = -tan(M_PI - angle);
		}
		else {
			preDirection.x = 0;
			preDirection.y = -1;
		}
		moveBall(preDirection);
	//}
}

void GameScene::collisionButtom(bool base) {

	auto angle = preDirection.getAngle();
	auto degree = CC_RADIANS_TO_DEGREES(-1 * angle);
	log("bottom    Degree:%f Angle:%f", degree, angle);
	if (preDirection.y < 0) {
		if (degree > 90) {
			preDirection.x = -1;
			preDirection.y = tan(angle - M_PI);
		}
		else if (degree < 90) {
			preDirection.x = 1;
			preDirection.y = tan(2 * M_PI - angle);
		}
		else {
			preDirection.x = 0;
			preDirection.y = 1;
		}
		//和接收盘接触的话还得考虑加速度
		if (base)
		{
			preDirection.x += horAcceration * Director::getInstance()->getAnimationInterval() / 3;
		}
		moveBall(preDirection);
	}
}

void GameScene::collisionLeft() {
	auto angle = preDirection.getAngle();
	auto degree = CC_RADIANS_TO_DEGREES(-1 * angle);
	log("left    Degree:%f Angle:%f", degree, angle);
	//if (ball->getPositionX() <= origin.x && preDirection.x < 0) {
		if (degree < -90) {
			preDirection.x = 1;
			preDirection.y = tan(M_PI - angle);
		}
		else if (degree > 90) {
			preDirection.x = 1;
			preDirection.y = -tan(angle - M_PI);
		}
		moveBall(preDirection);
	//}
}

void GameScene::collisionRight() {
	auto angle = preDirection.getAngle();
	auto degree = CC_RADIANS_TO_DEGREES(-1 * angle);
	log("right    Degree:%f Angle:%f", degree, angle);
	//if ((ball->getPositionX() >= origin.x + visible.width) && preDirection.x > 0) {
		if (degree < 0 && degree > -90) {
			preDirection.x = -1;
			preDirection.y = tan(angle);
			moveBall(preDirection);
		}
		if (degree > 0 && degree < 90)
		{
			preDirection.x = -1;
			preDirection.y = -tan(-angle);
			moveBall(preDirection);
		}
	//}
}

void GameScene::initGame() {
	isStart = false;
	totalBrick = 0;
	horAcceration = 0;
	preFlashCount = 0;
	curFlashCount = 0;
	ballSpeed = 200;

	bottom->setVisible(true);
	bottom->setAnchorPoint(Vec2(0.5, 0));
	bottom->setPosition(origin.x + visible.width / 2, origin.y);
	ball->stopAllActions();
	ball->setVisible(true);
	ball->setAnchorPoint(Vec2(0.5, 0));
	ball->setPosition(origin.x + visible.width / 2, origin.y + bottom->getContentSize().height);

	for (int i = 0; i < bricks.size(); i++) {
		auto vec = bricks.at(i);
		for (int j = 0; j < vec.size(); j++) {
			auto brick = vec.at(j);
			brick->stopAllActions();
			brick->setAnchorPoint(Vec2(0, 1));
			brick->setPosition(origin.x + j * brick->getContentSize().width,
				origin.y + visible.height - i * brick->getContentSize().height);
			brick->setVisible(true);
			brick->setOpacity(255);
			totalBrick++;
		}
	}
	log("Total:%d", totalBrick);
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
	addChild(ball, 2);
	addChild(bottom, 0);

	int countX = visible.width / brick->getContentSize().width;
	int countY = visible.height * 0.4 / brick->getContentSize().height;
	float sizeX = visible.width / countX;

	for (int i = 0; i < countY; i++) {
		Vector<Sprite *> vec;
		for (int j = 0; j < countX; j++) {
			brick = Sprite::create("brick.png");
			brick->setVisible(false);
			brick->setContentSize(Size(sizeX, brick->getContentSize().height));
			brick->setScaleX(sizeX / brick->getContentSize().width);
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

void GameScene::moveBall(Vec2 vec) {
	vec.x = vec.x * 2000;
	vec.y = vec.y * 2000;
	auto len = vec.distance(Vec2(0, 0));
	//log("x:%f y:%f len:%f time:%f", vec.x, vec.y, len, len / BALL_SPEED);
	ball->getColor();
	ball->setColor(Color3B(100, 79, 187));
	ball->stopAllActions();
	ball->runAction(MoveBy::create(len / ballSpeed, vec));
}

void GameScene::changeDirection(int direction, bool base) {
	//log("Angle:%f Degree:%f", angle, degree);
	//和底盘接触的情况
	switch (direction)
	{
	case 1:
		collisionTop();
		break;
	case 2:
		collisionButtom(base);
		break;
	case 3:
		collisionLeft();
		break;
	case 4:
		collisionRight();
		break;
	default:
		log("Illegal direction %d!", direction);
		break;
	}
}

void GameScene::update(float time) {

	if (!isStart) {
		return;
	}
	curFlashCount++;
	//首先监测球是否和底盘接触
	if (ball->boundingBox().intersectsRect(bottom->boundingBox())) {
		changeDirection(2, true);
		return;
	}
	if (ball->getPositionY() <= origin.y) 
	{
		//unscheduleUpdate();
		initGame();
		return;
	}

	//球碰撞到墙壁
	if (ball->getPositionX() <= origin.x && preDirection.x < 0) {
		changeDirection(3, false);
	}
	else if (ball->getPositionX() >= origin.x + visible.width && preDirection.x > 0) {
		changeDirection(4, false);
	}
	else if (ball->getPositionY() >= origin.y + visible.height && preDirection.y > 0) {
		changeDirection(1, false);
	}

	int count = 0;
	//每一帧监测是否碰撞到砖块
	for (int i = 0; i < bricks.size(); i++) {
		auto vec = bricks.at(i);
		for (int j = 0; j < vec.size(); j++) {
			auto brick = vec.at(j);
			if (brick->isVisible() && brick->getOpacity() == 255) {
				Rect bottomEdge = Rect(brick->getPositionX(), brick->getPositionY() - brick->getContentSize().height + 1,
					brick->getContentSize().width, 1);
				Rect topEdge = Rect(brick->getPositionX(), brick->getPositionY(),
					brick->getContentSize().width, 1);
				Rect leftEdge = Rect(brick->getPositionX(), brick->getPositionY() - brick->getContentSize().height + 1,
					1, brick->getContentSize().height);
				Rect rightEdge = Rect(brick->getPositionX() + brick->getContentSize().width - 1, brick->getPositionY() - brick->getContentSize().height + 1,
					1, brick->getContentSize().height);
				if (ball->boundingBox().intersectsRect(topEdge) && preDirection.y < 0) {
					changeDirection(2, false);
					brick->runAction(Sequence::create(FadeOut::create(0.5f), [=] {brick->setVisible(false);}, NULL));
					ballSpeed += ACC_SPEED;
				}
				else if (ball->boundingBox().intersectsRect(bottomEdge) && preDirection.y > 0) {
					changeDirection(1, false);
					brick->runAction(Sequence::create(FadeOut::create(0.5f), [=] {brick->setVisible(false);}, NULL));
					ballSpeed += ACC_SPEED;
				}
				else if (ball->boundingBox().intersectsRect(leftEdge) && preDirection.x > 0) {
					changeDirection(4, false);
					brick->runAction(Sequence::create(FadeOut::create(0.5f), [=] {brick->setVisible(false);}, NULL));
					ballSpeed += ACC_SPEED;
				}
				else if (ball->boundingBox().intersectsRect(rightEdge) && preDirection.x < 0) {
					changeDirection(3, false);
					brick->runAction(Sequence::create(FadeOut::create(0.5f), [=] {brick->setVisible(false);}, NULL));
					ballSpeed += ACC_SPEED;
				}
			}
			else {
				count++;
			}
		}
	}
	if (count >= totalBrick)
	{
		initGame();
	}
}