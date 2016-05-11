#include "GameScene.h"

bool GameScene::init() {
	if (!Layer::init()) {
		return false;
	}
	visible = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	auto bgSprite = Sprite::create("bg.png");
	bgSprite->setAnchorPoint(Vec2(0, 0));
	bgSprite->setPosition(Vec2(origin.x, origin.y));
	addChild(bgSprite, -1);

	auto map = TMXTiledMap::create("bg.tmx");
	map->setAnchorPoint(Vec2(0, 0));
	map->setPosition(Vec2(origin.x, origin.y));
	addChild(map, -1);

	player = Player::create(Vec2(origin.x + 40, origin.y + 70));
	addChild(player, 1);

	log("SpriteFrameCache");
	auto spriteCache = SpriteFrameCache::getInstance();
	spriteCache->addSpriteFramesWithFile("phoenix.plist", "phoenix.png");
	
	Vector<SpriteFrame *> vec;
	for (int i = 0; i < 16; i++) {

		log(i);
		char tmp[16];
		sprintf(tmp, "f-%d.png", i);
		auto frame = spriteCache->getSpriteFrameByName(tmp);
		vec.pushBack(frame);
		if (i % 4 == 3) {
			auto animation = Animation::createWithSpriteFrames(vec, 0.3f, -1);
			auto animate = Animate::create(animation);
			sprintf(tmp, "f-%d.png", i - 3);
			auto sprite = Sprite::createWithSpriteFrameName(tmp);
			sprite->setAnchorPoint(Vec2(0, 0));
			sprite->setPosition(Vec2(86 * (i / 4), 200));
			sprite->runAction(animate);
			addChild(sprite);
			vec.clear();
		}
	}

	TTFConfig config("fonts/Marker Felt.ttf", 30);
	auto label = Label::createWithTTF(config, "createLabelByTTF", TextHAlignment::CENTER);
	label->setString("Example!");
	label->enableGlow(Color4B::ORANGE);
	label->setAnchorPoint(Vec2(0.5, 0.5));
	label->setPosition(150, 100);
	addChild(label);



	auto listen = EventListenerTouchOneByOne::create();
	listen->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listen->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listen->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	auto dispatch = Director::getInstance()->getEventDispatcher();
	dispatch->addEventListenerWithSceneGraphPriority(listen, this);
	return true;
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event) {
	onTouchMoved(touch, unused_event);
	return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *unused_event) {
	curPos = touch->getLocation();
	if (!curPos.equals(prePos)) {
		player->rotateArrow(curPos);
	}
	prePos = curPos;
}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event) {
	if (!player->getIsRunAction()) {
		player->shootArrow(touch->getLocation());
	}
}

Scene *GameScene::createScene() {
	auto scene = Scene::create();
	auto gameLayer = GameScene::create();
	scene->addChild(gameLayer);

	return scene;
}