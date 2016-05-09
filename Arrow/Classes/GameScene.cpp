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
		player->rorateArrow(curPos);
	}
	prePos = curPos;
}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event) {

}

Scene *GameScene::createScene() {
	auto scene = Scene::create();
	auto gameLayer = GameScene::create();
	scene->addChild(gameLayer);

	return scene;
}