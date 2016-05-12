#include "GameScene.h"

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event) {
	birdSpeed = 200.0f;
	lastTime = -1;
	return true;
}
void GameScene::onTouchMoved(Touch *touch, Event *unused_event) {
}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event) {
}

bool GameScene::init() {
	if (!Layer::init()) {
		return false;
	}

	srand(time(0));
	visible = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	pipeBirdInterval = 90.0f;

	auto bgSprite = Sprite::create("bg.png");
	bgSprite->setAnchorPoint(Vec2(0, 0));
	bgSprite->setPosition(0, 0);
	addChild(bgSprite, -1);
	
	auto spriteCache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame *> vec;
	spriteCache->addSpriteFramesWithFile("bird.plist", "bird.png");
	for (int i = 1; i <= 4; i++) {
		char tmp[32];
		sprintf(tmp, "bird%d.bmp", i);
		auto frame = spriteCache->getSpriteFrameByName(tmp);
		vec.pushBack(frame);
	}
	auto animation = Animation::createWithSpriteFrames(vec, 0.2f, -1);
	auto animate = Animate::create(animation);

	birdSprite = Sprite::create("bird.jpg");
	birdSprite->setAnchorPoint(Vec2(0, 0.5));
	birdSprite->runAction(animate);
	addChild(birdSprite, 2);
	
	auto pipeTemplate = Sprite::create("pipe.jpg");
	PIPE_INTERVAL = pipeTemplate->getContentSize().width * 5.5;
	float beginX = birdSprite->convertToWorldSpace(Vec2(0, 0)).x + PIPE_INTERVAL;
	
	while (beginX <= origin.x + visible.width + 2 * PIPE_INTERVAL) {
		auto pipe = Sprite::create("pipe.jpg");
		pipe->setAnchorPoint(Vec2(0.5, 1));
		addChild(pipe, 1);
		pipeVec.pushBack(pipe);

		pipe = Sprite::create("pipe.jpg");
		pipe->setRotation(180);
		pipe->setAnchorPoint(Vec2(0.5, 1));
		addChild(pipe, 1);
		pipeVec.pushBack(pipe);

		beginX += PIPE_INTERVAL;
	}

	auto listen = EventListenerTouchOneByOne::create();
	listen->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listen->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listen->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	auto dispatch = Director::getInstance()->getEventDispatcher();
	dispatch->addEventListenerWithSceneGraphPriority(listen, this);

	initVariable();
	
	return true;
}

Scene* GameScene::createScene() {
	auto scene = Scene::create();
	auto game = GameScene::create();
	game->setPosition(0, 0);
	game->setAnchorPoint(Vec2(0, 0));
	scene->addChild(game, -1);
	return scene;
}

void GameScene::update(float time) {
	flashCount++;
	float limit = 1 / Director::getInstance()->getAnimationInterval();
	float height = visible.height - birdSprite->getContentSize().height - pipeBirdInterval;

	runTime += 1 / limit;
	if (lastTime <= 0.0) {
		lastTime = runTime;
	}
	else {
		auto val = runTime - lastTime;
		auto newSpeed = birdSpeed + val * aggregation;
		auto distance = birdSpeed * val + 0.5 * aggregation * val * val;
		birdSpeed = newSpeed;
		birdSprite->setPosition(birdSprite->getPositionX(), birdSprite->getPositionY() + distance);
		lastTime = runTime;
	}
	auto collisionBtm = pipeVec.at(collisionIndex);
	auto collisionTop = pipeVec.at(collisionIndex + 1);
	if (birdSprite->boundingBox().intersectsRect(collisionBtm->boundingBox()) ||
		birdSprite->boundingBox().intersectsRect(collisionTop->boundingBox())) {
		unscheduleUpdate();
		return;
	}

	for (int i = 0; i < pipeVec.size(); i += 2) {
		auto bottom = (Sprite *)pipeVec.at(i);
		auto top = (Sprite *)pipeVec.at(i + 1);

		if (flashCount < limit) {
			continue;
		}
		auto ranX = bottom->getPositionX() - SPEED;
		auto ranY = bottom->getPositionY();
		if (bottom->getPositionX() + bottom->getContentSize().width / 2 < origin.x) {
			if (i == 0) {
				ranX = pipeVec.back()->getPositionX() + PIPE_INTERVAL - SPEED;
			}
			else {
				ranX = pipeVec.at(i - 1)->getPositionX() + PIPE_INTERVAL - SPEED;
			}
			ranY = getRandomY(height, 0.8);
			bottom->setPosition(ranX, origin.y + ranY);
			top->setPosition(ranX, origin.y + ranY + pipeBirdInterval);

			collisionIndex = (collisionIndex + 2) % pipeVec.size();
			//log("%d", collisionIndex);
		}
		else {
			bottom->runAction(MoveTo::create(1.0f, Vec2(ranX, origin.y + ranY)));
			top->runAction(MoveTo::create(1.0f, Vec2(ranX, origin.y + ranY + pipeBirdInterval)));
		}
	}
	
	if (flashCount >= limit) {
		flashCount = 0;
	}
}

void GameScene::initVariable() {
	aggregation = -350.0f;
	birdSpeed = 0.0f;
	lastTime = -1;
	runTime = 0.0f;
	flashCount = 0;
	collisionIndex = 0;

	birdSprite->setPosition(origin.x, origin.y + visible.height / 2);
	
	float height = visible.height - birdSprite->getContentSize().height - pipeBirdInterval;
	float ranX = birdSprite->convertToWorldSpace(Vec2(0, 0)).x + PIPE_INTERVAL;
	for (int i = 0; i < pipeVec.size(); i += 2) {
		auto bottom = pipeVec.at(i);
		auto top = pipeVec.at(i + 1);
		auto ranY = getRandomY(height, 0.8);
		bottom->setPosition(ranX, ranY);
		top->setPosition(ranX, ranY + pipeBirdInterval);
		
		ranX += PIPE_INTERVAL;
	}
	scheduleUpdate();
}

float GameScene::getRandomY(float height, float prab) {
	float ranY = 0;
	do {
		ranY = rand() % (int)(height * prab);
	} while (ranY < height * (1- prab));
	return ranY;
}