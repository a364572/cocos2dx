#include "GameScene.h"
GameScene::GameScene()
{
}
GameScene::GameScene(std::string name)
{
	this->setSceneName(name);
}
GameScene* GameScene::create(std::string name)
{
	auto gameScene = new GameScene(name);
	if (gameScene && gameScene->init())
	{
		gameScene->autorelease();
		return gameScene;
	}
	delete gameScene;
	return nullptr;
}

Scene* GameScene::createScene(std::string name)
{
	auto scene = Scene::create();
	auto layer = create(name);
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init()) {
		return false;
	}
	visible = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	int val = 1;
	tmx = TMXTiledMap::create(sceneName);
	auto bg = tmx->getLayer("background");
	tmx->setAnchorPoint(Vec2(0.5, 0.5));
	tmx->setPosition(origin.x + visible.width / 2, origin.y + visible.height / 2);
	//offX = (visible.width - tmx->getContentSize().width) / 2;
	offX = 0;
	addChild(tmx);
	auto obj = tmx->getObjectGroup("points");
	auto point = obj->getObject(std::to_string(val));
	auto ite = point.begin();
	while (point.begin() != point.end())
	{
		auto x = point.at("x").asFloat() + offX;
		auto y = point.at("y").asFloat();
		log("x:%f, y:%f", x, y);
		auto node = Node::create();
		node->setPosition(x, y);
		positions.pushBack(node);
		val++;
		point = obj->getObject(std::to_string(val));
	}
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("monster.plist", "monster.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("TBottle-hd.plist", "TBottle-hd.png");
	monsterInterval = 3;
	flashCount = 0;

	auto tower = BottleTower::create();
	tower->setPosition(150, 200);
	tower->visible = tmx->getContentSize();
	tower->origin = Vec2(-tower->getPositionX(), -tower->getPositionX());
	tmx->addChild(tower, 2);
	towers.pushBack(tower);
	scheduleUpdate();
}

void GameScene::update(float dt)
{
	flashCount++;
	if (Director::getInstance()->getAnimationInterval() * flashCount >= monsterInterval)
	{
		flashCount = 0;
		auto monster = Monster::create("f-5.png", 50, 100, positions);
		monsters.pushBack(monster);
		monster->forwad();
		tmx->addChild(monster, 1);
	}

	for (int j = 0; j < towers.size(); j++)
	{
		//首先旋转炮台
		auto tower = towers.at(j);
		tower->setRotation(tower->getRotation() + 2);
	}
	for (int i = 0; i < monsters.size(); i++)
	{
		auto monster = monsters.at(i);
		if (monster->getCurrentIndex() == positions.size())
		{
			monsters.erase(i);
			i--;
			monster->removeFromParentAndCleanup(true);
			continue;
		}
	}

}