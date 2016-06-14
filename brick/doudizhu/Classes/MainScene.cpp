#include "MainScene.h"
#include "DeckScene.h"
Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

bool MainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	origin = Director::getInstance()->getVisibleOrigin();
	visible = Director::getInstance()->getVisibleSize();
	auto winSize = Director::getInstance()->getWinSize();
	auto winSizePixel = Director::getInstance()->getWinSizeInPixels();

	log("origin:%f %f", origin.x, origin.y);
	log("visible:%f %f", visible.width, visible.height);
	log("winSize:%f %f", winSize.width, winSize.height);
	log("winSizeInPixek:%f %f", winSizePixel.width, winSizePixel.height);
	//增加背景图片 按钮等
	auto background = Sprite::create("background.png");
	background->setPosition(origin.x + visible.width / 2, origin.y + visible.height / 2);
	this->addChild(background, -1);

	auto logo = Sprite::create("logo.png");
	logo->setPosition(origin.x + visible.width / 3, origin.y + visible.height / 2);
	background->addChild(logo);

	auto startMenu = MenuItemImage::create("go_game_btn.png", "go_game_btn_down.png", "", CC_CALLBACK_0(MainScene::startGame, this));
	auto helpMenu = MenuItemImage::create("start_green_btn.png", "start_green_btn_down.png", "", CC_CALLBACK_0(MainScene::displayHelp, this));
	auto exitMenu = MenuItemImage::create("start_red_btn.png", "start_red_btn_down.png", "", CC_CALLBACK_0(MainScene::exitGame, this));

	mainMenu = Menu::create();
	mainMenu->addChild(startMenu);
	mainMenu->addChild(helpMenu);
	mainMenu->addChild(exitMenu);
	mainMenu->alignItemsVertically();
	mainMenu->setAnchorPoint(Vec2(0.5, 0.5));
	mainMenu->setPosition(origin.x + visible.width / 3 * 2, origin.y + visible.height / 2);
	background->addChild(mainMenu);

	helpSprite = nullptr;
	return true;
}

void MainScene::startGame()
{
	Director::getInstance()->replaceScene(DeckScene::createScene());
}

void MainScene::displayHelp()
{
	if (helpSprite == nullptr) {
		helpSprite = Sprite::create("aboutGame.png");
		auto backBtn = MenuItemSprite::create(Sprite::create("choose_game_exit_btn.png"), 
			Sprite::create("choose_game_exit_btn.png"), CC_CALLBACK_0(MainScene::backFromHelp, this));
		auto menu = Menu::create(backBtn, nullptr);
		menu->setPosition(origin.x + 50, origin.y + 40);
		helpSprite->setPosition(origin.x + visible.width / 2, origin.y + visible.height / 2);
		helpSprite->addChild(menu);
		addChild(helpSprite);
	}
	helpSprite->setVisible(true);
	mainMenu->setEnabled(false);
}

void MainScene::exitGame()
{
	Director::getInstance()->end();
}

void MainScene::backFromHelp()
{
	helpSprite->setVisible(false);
	mainMenu->setEnabled(true);
}

