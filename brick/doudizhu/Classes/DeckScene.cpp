#include "DeckScene.h"
#include "GameManager.h"
Scene* DeckScene::createScene()
{
	auto scene = Scene::create();
	auto deck = DeckScene::create();
	scene->addChild(deck);
	return scene;
}

bool DeckScene::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	visible = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	//首先预加载资源
	
	auto bg = Sprite::create("background.png");
	bg->setPosition(origin.x + visible.width / 2, origin.y + visible.height / 2);
	addChild(bg, -1);
	blackVertical = (Director::getInstance()->getWinSize().height - bg->getContentSize().height) / 2;
	blackHorizon = (Director::getInstance()->getWinSize().width - bg->getContentSize().width) / 2;
	if (blackVertical < 0)
	{
		blackVertical = 0;
	}
	if (blackHorizon < 0)
	{
		blackHorizon = 0;
	}

	progressBar = ProgressTimer::create(Sprite::create("progress.png"));
	progressBar->setAnchorPoint(Vec2(0.5, 0.5));
	progressBar->setType(ProgressTimer::Type::BAR);
	progressBar->setMidpoint(Vec2(0, 0.5));
	progressBar->setBarChangeRate(Vec2(1, 0));
	progressBar->setPercentage(0);

	progress = Sprite::create("progress_bg.png");
	progressBar->setPosition(progress->getContentSize().width / 2, progress->getContentSize().height / 2);
	progress->setPosition(origin.x + visible.width / 2, origin.y + visible.height / 2);
	progress->addChild(progressBar);
	addChild(progress);
	
	loadLabel = Label::createWithTTF("load", "fonts/Marker Felt.ttf", 40);
	loadLabel->setColor(Color3B(126, 106, 79));
	loadLabel->setString("Loading...");
	loadLabel->setAnchorPoint(Vec2(0.5, 0.5));
	loadLabel->setPosition(progress->getPosition() + Vec2(0, 50));
	addChild(loadLabel);
	schedule(schedule_selector(DeckScene::preload));
	return true;
}

//在界面上显示玩家 准备开始游戏
void DeckScene::initDeck()
{
	log("Display Card!");
	for (PokerCard *card : cardArray) {
		card->removeFromParent();
	}
	cardArray.clear();
	int row = -1;
	int col = 0;
	for (int i = 0; i < 54; i++)
	{
		auto card = GameManager::getInstance()->getRawCardArray().at(i);
		card->setAnchorPoint(Vec2(0, 0));
		cardArray.pushBack(card);
		if (i % 13 == 0) {
			row++;
			col = 0;
		}
		card->setPosition(col * card->getContentSize().width, row * card->getContentSize().height);
		addChild(card);
		col++;
	}
	for (int i = 0; i < 54; i++)
	{
		auto card = cardArray.at(i);
		//card->runAction(FadeOut::create(i + 1));
	}

	float margin_top = 50 + blackVertical;
	float margin_right = 50 + blackHorizon;
	float margin_left = 50 + blackHorizon;
	auto playerBackGold = Sprite::create("logo_active.png");
	auto playerBackGray1 = Sprite::create("logo_deactive.png");
	auto playerBackGray2 = Sprite::create("logo_deactive.png");
	playerBackGold ->setPosition(0, 0);
	playerBackGold ->setAnchorPoint(Vec2(0, 0));
	playerBackGray1->setPosition(0, 0);
	playerBackGray1->setAnchorPoint(Vec2(0, 0));
	playerBackGray2->setPosition(0, 0);
	playerBackGray2->setAnchorPoint(Vec2(0, 0));

	auto player = Sprite::create("logo_unknown.png");
	auto player1 = Sprite::create("logo_unknown.png");
	auto player2 = Sprite::create("logo_unknown.png");

	player->addChild(playerBackGold);
	player1->addChild(playerBackGray1);
	player2->addChild(playerBackGray2);

	player->setAnchorPoint(Vec2(0, 1));
	player->setPosition(origin.x + margin_left, origin.y + visible.height / 2 - 50);

	player1->setAnchorPoint(Vec2(0, 1));
	player1->setPosition(origin.x + margin_left, origin.y + visible.height - margin_top);

	player2->setAnchorPoint(Vec2(1, 1));
	player2->setPosition(origin.x + visible.width - margin_right, origin.y + visible.height - margin_top);

	addChild(player);
	addChild(player1);
	addChild(player2);
	auto exitBtn = MenuItemImage::create("game_exit.png", "game_exit.png", CC_CALLBACK_0(DeckScene::exitGame, this));
	auto setBtn = MenuItemImage::create("game_setting.png", "game_setting.png", CC_CALLBACK_0(DeckScene::displaySetting, this));
	auto menu = Menu::create(exitBtn, setBtn, nullptr);
	menu->alignItemsHorizontallyWithPadding(20);
	menu->setAnchorPoint(Vec2(0, 0.5));
	menu->setPosition(player1->getPositionX() + player1->getContentSize().width + 150, player1->getPositionY() - player1->getContentSize().height / 2);

	auto pokerBack1 = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("poke_back_header.png"));
	auto pokerBack2 = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("poke_back_header.png"));
	auto pokerBack3 = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("poke_back_header.png"));
	auto pokerBack4 = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("poke_back_header.png"));
	auto pokerBack5 = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("poke_back_header.png"));

	pokerBack1->setScale(player1->getContentSize().height / pokerBack1->getContentSize().height);
	pokerBack2->setScale(player1->getContentSize().height / pokerBack1->getContentSize().height);
	pokerBack3->setScale(player1->getContentSize().height / pokerBack1->getContentSize().height);
	pokerBack4->setScale(player1->getContentSize().width / pokerBack1->getContentSize().width);
	pokerBack5->setScale(player1->getContentSize().width / pokerBack1->getContentSize().width);

	pokerBack2->setAnchorPoint(Vec2(0.5, 1));
	pokerBack2->setPosition(origin.x + visible.width / 2, origin.y + visible.height - margin_top);
	pokerBack1->setAnchorPoint(Vec2(0.5, 1));
	pokerBack1->setPosition(pokerBack2->getPosition() - Point(pokerBack2->getContentSize().width, 0));
	pokerBack3->setAnchorPoint(Vec2(0.5, 1));
	pokerBack3->setPosition(pokerBack2->getPosition() + Point(pokerBack2->getContentSize().width, 0));

	pokerBack4->setAnchorPoint(Vec2(0, 1));
	pokerBack4->setPosition(player1->getPositionX(), player1->getPositionY() - player1->getContentSize().height - 20);
	pokerBack5->setAnchorPoint(Vec2(1, 1));
	pokerBack5->setPosition(player2->getPositionX(), player2->getPositionY() - player2->getContentSize().height - 20);
	
	addChild(pokerBack1);
	addChild(pokerBack2);
	addChild(pokerBack3);
	addChild(pokerBack4);
	addChild(pokerBack5);

	auto readyBtn = MenuItemImage::create("big_green_btn.png", "big_green_btn_down.png", CC_CALLBACK_0(DeckScene::startGame, this));
	auto readyMenu = Menu::create(readyBtn, nullptr);


	//auto readyMenu =
//	menu->setColor(Color3B(172, 64, 90));
//	log("Menu Size : %f %f", menu->getContentSize().width, menu->getContentSize().height);
//	log("Menu AnchorPoint: %f %f", menu->getAnchorPoint().x, menu->getAnchorPoint().y);
//	log("Menu Point: %f %f", menu->getPositionX(), menu->getPositionY());
//	log("Exit AnchorPoint: %f %f", exitBtn->getAnchorPoint().x, exitBtn->getAnchorPoint().y);
//	log("Exit Point: %f %f", exitBtn->getPositionX(), exitBtn->getPositionY());
//	log("Setting AnchorPoint: %f %f", setBtn->getAnchorPoint().x, setBtn->getAnchorPoint().y);
//	log("Setting Point: %f %f", setBtn->getPositionX(), setBtn->getPositionY());

	addChild(menu);
}

void DeckScene::preload(float dt)
{
	auto manager = GameManager::getInstance();
	int percent = manager->numberOfLoadRes * 100 / manager->numberOfTotalRes;

	progressBar->setPercentage(percent);
	loadLabel->setString("Loading " + std::to_string(percent) + "%");
	if (manager->numberOfLoadRes >= manager->numberOfTotalRes)
	{
		unschedule(schedule_selector(DeckScene::preload));
		this->runAction(Sequence::create(DelayTime::create(1.0f), 
			CallFunc::create([=] 
			{
				progress->removeFromParentAndCleanup(true); 
				loadLabel->removeFromParentAndCleanup(true);
				initDeck(); 
			}), 
			nullptr));
	}
}

void DeckScene::startGame()
{
}

void DeckScene::exitGame()
{
}

void DeckScene::displaySetting()
{
}
