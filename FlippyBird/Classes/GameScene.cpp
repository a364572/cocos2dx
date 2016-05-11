#include "GameScene.h"
#include "cocos-ext.h"
bool GameScene::init() {
	if (!Layer::init()) {
		return false;
	}

	auto bgSprite = Sprite::create("bg.png");
	addChild(bgSprite, -1);

	auto birdSprite = Sprite::create("bird.png");
	birdSprite->setAnchorPoint(Vec2(0, 0.5));
	birdSprite->setPosition(origin.x, origin.y + visible.height / 2);
	addChild(birdSprite, 1);

	//auto pipeTemplate = Scale9Sprite::create("pipe.png");
	int beginX = birdSprite->getContentSize().width + 20;

	
	while (beginX <= visible.width) {
		auto pipe = Sprite::create("pipe.png");

	}

	return true;
}

Scene* GameScene::createScene() {
	auto scene = Scene::create();
	auto game = GameScene::create();
	scene->addChild(game, -1);
	return scene;
}