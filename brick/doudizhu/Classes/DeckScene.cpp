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
	
	progressBar = ProgressTimer::create(Sprite::create("progress.png"));
	progressBar->setAnchorPoint(Vec2(0.5, 0.5));
	progressBar->setPosition(progressBar->getContentSize().width / 2, progressBar->getContentSize().height / 2);
	progressBar->setType(ProgressTimer::Type::BAR);
	progressBar->setMidpoint(Vec2(0, 0.5));
	progressBar->setBarChangeRate(Vec2(1, 0));
	progressBar->setPercentage(0);

	progress = Sprite::create("progress_bg.png");
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

void DeckScene::initDeck()
{

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
		initDeck();
	}
	else
	{
	}
}
