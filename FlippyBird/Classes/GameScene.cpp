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

	scoreLabel = Label::create();
	scoreLabel->setAnchorPoint(Vec2(0, 1));
	scoreLabel->setPosition(origin.x, origin.y + visible.height);
	addChild(scoreLabel, 3);

	TTFConfig config("fonts/Marker Felt.ttf", 30);
	auto startLabel = Label::createWithTTF(config, "Start", TextHAlignment::CENTER);
	startLabel->enableGlow(Color4B::MAGENTA);
	auto menuRestart = MenuItemLabel::create(startLabel, CC_CALLBACK_1(GameScene::startGame, this));
	menuRestart->setPosition(0, 50);

	auto exitLabel = Label::createWithTTF(config, "Exit", TextHAlignment::CENTER);
	exitLabel->enableGlow(Color4B::ORANGE);
	auto menuExit = MenuItemLabel::create(exitLabel, CC_CALLBACK_1(GameScene::stopGame, this));
	menuExit->setPosition(0, 0);

	menu = Menu::create(menuRestart, menuExit, NULL);
	menu->setAnchorPoint(Vec2(0.5, 0.5));
	menu->setPosition(visible.width / 2 + origin.x, visible.height / 2 + origin.y);
	addChild(menu, 3);

	auto animation = Animation::createWithSpriteFrames(vec, 0.2f, -1);
	auto animate = Animate::create(animation);

	birdSprite = Sprite::create("bird.jpg");
	birdSprite->setAnchorPoint(Vec2(0, 0.5));
	birdSprite->runAction(animate);
	addChild(birdSprite, 2);
	
	auto pipeTemplate = Sprite::create("pipe.jpg");
	PIPE_INTERVAL = pipeTemplate->getContentSize().width * 5;
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
		birdSprite->boundingBox().intersectsRect(collisionTop->boundingBox()) ||
		colllisionWithWall()) {
		//birdSprite->stopAllActions();
		for (int i = 0; i < pipeVec.size(); i++) {
			pipeVec.at(i)->stopAllActions();
		}
		unscheduleUpdate();
		menu->setVisible(true);
		return;
	}

	//鸟的位置超过了水管，分数加1
	if (previousPipe != collisionIndex && 
		collisionBtm->getPositionX() + collisionBtm->getContentSize().width / 2 < origin.x) {
		score++;
		char tmp[16];
		sprintf(tmp, "%04d", score);
		scoreLabel->setString(tmp);
		previousPipe = collisionIndex;
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
	previousPipe = -1;
	score = 0;

	menu->setVisible(false);
	scoreLabel->setString("0000");

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

void GameScene::startGame(Ref* ref) {
	initVariable();
	menu->setVisible(false);
}

void GameScene::stopGame(Ref *ref) {
	Director::getInstance()->end();
}
bool GameScene::colllisionWithWall() {
	float y = (birdSprite->convertToWorldSpace(Vec2(0, 0))).y;
	if (y - birdSprite->getContentSize().height / 2 <= origin.y ||
		y + birdSprite->getContentSize().height / 2 >= origin.y + visible.height) {
		return true;
	}
	return false;
}

float GameScene::getRandomY(float height, float prab) {
	float ranY = 0;
	do {
		ranY = rand() % (int)(height * prab);
	} while (ranY < height * (1- prab));
	return ranY;
}