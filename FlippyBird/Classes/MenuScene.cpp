#include "MenuScene.h"

void MenuScene::startGame(Ref* ref) {
	log("start game");
}

void MenuScene::endGame(Ref* ref) {
	log("end game");
	Director::getInstance()->end();
}

bool MenuScene::init() {
	if (!Layer::init()) {
		return false;
	}
	visible = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	auto beginItem = MenuItemFont::create("Begin", this, menu_selector(MenuScene::startGame));
	beginItem->setAnchorPoint(Vec2(0.5f, 0.5f));
	beginItem->setPosition(origin.x + visible.width / 2, origin.y + visible.height / 3 * 2);

	auto exitItem = MenuItemFont::create("Exit", this, menu_selector(MenuScene::endGame));
	exitItem->setAnchorPoint(Vec2(0.5f, 0.5f));
	exitItem->setPosition(origin.x + visible.width / 2, origin.y + visible.height / 3);
	auto menu = Menu::create(beginItem, exitItem, NULL);
	menu->setPosition(0, 0);
	menu->setAnchorPoint(Vec2(0.5f, 0.5f));
	addChild(menu);

	return true;
}

Scene* MenuScene::createScene() {
	auto scene = Scene::create();
	auto menu = MenuScene::create();
	scene->addChild(menu);
	return scene;
}