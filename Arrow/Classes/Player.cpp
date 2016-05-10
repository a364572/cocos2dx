#include "Player.h"

Player::Player() {
}

Player* Player::create(Vec2 pos) {
	auto player = new Player();
	if (player && player->init(pos)) {
		player->autorelease();
		return player;
	}
	delete player;
	player = NULL;
	return NULL;
}

bool Player::init(Vec2 pos) {
	if (!Sprite::init()) {
		return false;
	}
	this->playerPos = pos;
	isRunAction = false;
	arrowArr = Array::create();
	arrowArr->retain();
	createPlayer();
	createPlayerHP();
	scheduleUpdate();
	return true;
}

void Player::createPlayer() {
	playerBodySprite = Sprite::create("player.png");
	playerBodySprite->setAnchorPoint(Vec2(0.5, 0.5));
	playerBodySprite->setPosition(playerPos);
	addChild(playerBodySprite);

	arrowSprite = Sprite::create("arrow.png");
	arrowSprite->setAnchorPoint(Vec2(0, 0.5));
	arrowSprite->setPosition(Vec2(playerBodySprite->getContentSize().width - 7,
		playerBodySprite->getContentSize().height / 2));
	playerBodySprite->addChild(arrowSprite);
}

void Player::createPlayerHP() {
	hpSprite = Sprite::create("xuetiao_empty.png");
	hpSprite->setAnchorPoint(Vec2(0.5, 0.5));
	hpSprite->setPosition(Vec2(playerBodySprite->getContentSize().width / 2, playerBodySprite->getContentSize().height + 10));
	playerBodySprite->addChild(hpSprite);

	hpBar = ProgressTimer::create(Sprite::create("xuetiao_full.png"));
	hpBar->setType(ProgressTimer::Type::BAR);
	hpBar->setMidpoint(Vec2(0, 0.5));
	hpBar->setPercentage(100);
	hpBar->setBarChangeRate(Vec2(1, 0));
	hpBar->setPosition(Vec2(0, 0));
	hpBar->setAnchorPoint(Vec2(0, 0));
	hpSprite->addChild(hpBar);
}

void Player::rorateArrow(Point touch) {
	Vec2 diff = touch - playerBodySprite->getPosition();
	auto angle = diff.getAngle();
	auto degree = CC_RADIANS_TO_DEGREES(-1 * angle);

	if (degree >= 90 && degree <= 180) {
		degree = 90;
	}
	if (degree >= -180 && degree <= -90) {
		degree = -90;
	}
	auto speed = 0.5 / M_PI;
	auto duration = fabs(angle * speed);

	arrowSprite->runAction(RotateTo::create(duration, degree));
}

void Player::finishShoot(Sprite *arrow) {
	//arrow->removeFromParentAndCleanup(true);
	log("Y: %f", arrow->getPositionX());
	isRunAction = false;
}
/*
void Player::createAndShootArrow(Point point) {
	isRunAction = true;
	auto animation = AnimationCache::getInstance()->getAnimation("player");
	auto animate = Animate::create(animation);

	auto funcall1 = CallFunc::create(CC_CALLBACK_0(Player::shootArrow, this));
	auto delay = DelayTime::create(0.5f);
	auto funcall2 = CallFunc::create(CC_CALLBACK_0(Player::finishShoot, this));
	playerBodySprite->runAction(Sequence::create(animate, funcall1, delay, funcall2, NULL));
}
*/
void Player::shootArrow(Point touch) {
	isRunAction = true;
	auto arrow = Sprite::create("arrow1.png");
	arrow->setPosition(Vec2(arrowSprite->getPositionX(),
		arrowSprite->getPositionY()));
	playerBodySprite->addChild(arrow);

	Point world = arrow->convertToWorldSpace(Vec2(0, 0));
	auto path = ArrowPath::create(2.0f, touch, Vec2(touch.x * 2 - world.x, 0));
	arrow->runAction(Sequence::create(path, NULL));
	arrowArr->addObject(arrow);
	isRunAction = false;
}

void Player::update(float delat) {
	for (int i = 0; i < arrowArr->count(); i++) {
		auto arrow = (Sprite *)arrowArr->getObjectAtIndex(i);
		if (!arrow->isRunning()) {
			arrowArr->removeObjectAtIndex(i);
			i--;
			arrow->removeFromParent();
			continue;
		}
	}
}
