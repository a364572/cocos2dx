#include "MainScene.h"
#include "GameScene.h"
void MainScene::startGame(Ref *ref) {
	Director::getInstance()->replaceScene(GameScene::createScene());
}
void MainScene::exitGame(Ref *ref) {

}
Scene* MainScene::createScene() {
	auto scene = Scene::create();
	auto main = MainScene::create();
	scene->addChild(main);

	return scene;
}
bool MainScene::init() {
	if (!Layer::init()) {
		return false;
	}

	visible = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	auto bgLayer = LayerColor::create(Color4B(126, 50, 157, 10), origin.x + visible.width, origin.y + visible.height);
	bgLayer->setAnchorPoint(Vec2(0, 0));
	bgLayer->setPosition(0, 0);
	addChild(bgLayer, -1);

	TTFConfig config("fonts/Marker Felt.ttf", 50);
	auto startLabel = Label::createWithTTF(config, "Start", TextHAlignment::CENTER);
	startBtn = MenuItemLabel::create(startLabel, CC_CALLBACK_1(MainScene::startGame, this));
	//startBtn->setAnchorPoint(Vec2(0, 0));
	//startBtn->setPosition(0, 80);

	auto exitLabel = Label::createWithTTF(config, "Exit", TextHAlignment::CENTER);
	exitBtn = MenuItemLabel::create(exitLabel, CC_CALLBACK_1(MainScene::exitGame, this));
	//exitBtn->setAnchorPoint(Vec2(0, 0));
	//exitBtn->setPosition(0, 0);

	mainMenu = Menu::create(startBtn, exitBtn, NULL);
	mainMenu->setAnchorPoint(Vec2(0.5, 0.5));
	mainMenu->setPosition(origin.x + visible.width / 2, origin.y + visible.height / 2);
	mainMenu->alignItemsVertically();
	addChild(mainMenu, 0);

	return true;
}