#include "GameRoomScene.h"
#include "GameManager.h"
#include "DeckScene.h"

Scene * GameRoomScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameRoomScene::create();
	scene->addChild(layer);
	return scene;
}

void GameRoomScene::enterRoom()
{
	if (selectIndex < 0 || selectIndex >= GameManager::getInstance()->roomList.size())
	{
		MessageBox("未选中房间", "错误");
			return;
	}
	if (isWaitingEnter)
	{
		return;
	}
	auto room = GameManager::getInstance()->roomList.at(selectIndex).name;
	log("ready to enter room %s", room.data());
	
	isWaitingEnter = true;
	enterFlashCount = 0;
	GameManager::getInstance()->enterRoom(room);
}

void GameRoomScene::createRoom()
{
	if (isWaitingEnter)
	{
		return;
	}
	isWaitingEnter = true;
	enterFlashCount = 0;
	GameManager::getInstance()->createRoom(GameManager::getInstance()->player);
}

void GameRoomScene::refreshRoom()
{
	if (isWaitingList)
	{
		return;
	}
	isWaitingList = true;
	listFlashCount = 0;
	GameManager::getInstance()->getRoomList();
}

void GameRoomScene::refreshListView()
{
	selectIndex = -1;
	listView->removeAllItems();
	for (int i = 0; i < GameManager::getInstance()->roomList.size(); i++)
	{
		auto widget = ui::Layout::create();
		auto buttonSprite = Sprite::create("room.png");
		auto nameText = ui::Text::create(GameManager::getInstance()->roomList.at(i).name, "arial", 30);
		auto countText = ui::Text::create(std::to_string(GameManager::getInstance()->roomList.at(i).count), "arial", 30);
		buttonSprite->setAnchorPoint(Vec2(0, 0));
		buttonSprite->setPosition(Vec2::ZERO);
		widget->setContentSize(buttonSprite->getContentSize() * 1.2);
		widget->addChild(buttonSprite);
		widget->setTouchEnabled(true);

		nameText->setAnchorPoint(Vec2(0, 0.5));
		nameText->setPosition(Vec2(0, widget->getContentSize().height / 2));
		countText->setAnchorPoint(Vec2(1, 0.5));
		countText->setPosition(Vec2(widget->getContentSize().width, widget->getContentSize().height / 2));
		widget->addChild(nameText);
		widget->addChild(countText);
		listView->pushBackCustomItem(widget);
	}
}

void GameRoomScene::roomListSelected(Ref * ref, ui::ListView::EventType type)
{
	auto list = (ui::ListView *)ref;
	switch (type)
	{
	case ui::ListView::EventType::ON_SELECTED_ITEM_START:
		log("Item %d Selected", list->getCurSelectedIndex());
		selectIndex = list->getCurSelectedIndex();
		break;
	case ui::ListView::EventType::ON_SELECTED_ITEM_END:
		break;
	}
}

void GameRoomScene::roomListScrolled(Ref * ref, ui::ScrollView::EventType type)
{
	switch (type)
	{
	case ui::ScrollView::EventType::SCROLL_TO_TOP:
		log("Scroll to top");
		break;
	case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:
		log("Scroll to bottom");
		break;
	}

}

bool GameRoomScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visible = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	selectIndex = -1;
	listFlashCount = 0;
	enterFlashCount = 0;
	isWaitingList = false;
	isWaitingEnter = false;


	GameManager::getInstance();

	log("Add background.png");
	auto bg = Sprite::create("background.png");
	bg->setPosition(origin.x + visible.width / 2, origin.y + visible.height / 2);
	addChild(bg, -1);

	//创建listview 分别注册滑动事件和点击事件
	listView = ui::ListView::create();
	listView->setDirection(ui::SCROLLVIEW_DIR_VERTICAL);
	listView->ignoreAnchorPointForPosition(false);
	listView->setTouchEnabled(true);
	listView->setBounceEnabled(true);
	listView->setBackGroundColor(Color3B(255, 0, 0));
	listView->setSize(Size(300, 400));
	listView->setAnchorPoint(Vec2(0.5, 0.5));
	listView->setPosition(Vec2(origin.x + visible.width / 2, origin.y + visible.height / 2));
	listView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(GameRoomScene::roomListSelected, this));
	listView->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(GameRoomScene::roomListScrolled, this));
	
	addChild(listView);
	TTFConfig ttf("fonts/arial.ttf", 50);

	auto refreshRoomMenu = MenuItemLabel::create(Label::createWithTTF(ttf, "Refresh"));
	refreshRoomMenu->setCallback(CC_CALLBACK_0(GameRoomScene::refreshRoom, this));

	auto createRoomMenu = MenuItemLabel::create(Label::createWithTTF(ttf, "Create"));
	createRoomMenu->setCallback(CC_CALLBACK_0(GameRoomScene::createRoom, this));

	auto enterRoomMenu = MenuItemLabel::create(Label::createWithTTF(ttf, "Enter"));
	enterRoomMenu->setCallback(CC_CALLBACK_0(GameRoomScene::enterRoom, this));

	mainMenu = Menu::create(refreshRoomMenu, createRoomMenu, enterRoomMenu, nullptr);
	mainMenu->alignItemsVerticallyWithPadding(10);
	mainMenu->setAnchorPoint(Vec2(0.5, 0.5));
	mainMenu->setPosition(origin.x + visible.width * 2 / 3, origin.y + visible.height / 2);
	addChild(mainMenu);
	scheduleUpdate();
	return true;
}

void GameRoomScene::update(float delta)
{
	if (this->isWaitingList)
	{
		listFlashCount++;
		if (Director::getInstance()->getAnimationInterval() * listFlashCount >= CONNECT_TIME_OUT / 1000)
		{
			GameManager::getInstance()->isWaitingList = 0;
			this->isWaitingList = false;
			this->listFlashCount = 0;
			MessageBox("加载房间列表超时", "错误");
		}
		else
		{
			int result = GameManager::getInstance()->isWaitingList;
			//需要更新房间列表
			if (result == 2)
			{
				GameManager::getInstance()->isWaitingList = 0;
				this->isWaitingList = false;
				this->listFlashCount = 0;
				refreshListView();
			}
			else if (result == 3)
			{
				GameManager::getInstance()->isWaitingList = 0;
				this->isWaitingList = false;
				this->listFlashCount = 0;
				MessageBox("加载房间列表失败", "错误");
			}
			else if (result != 1)
			{
				GameManager::getInstance()->isWaitingList = 0;
				this->isWaitingList = false;
				this->listFlashCount = 0;
			}
		}
	}

	if (isWaitingEnter)
	{
		enterFlashCount++;
		if (Director::getInstance()->getAnimationInterval() * enterFlashCount >= CONNECT_TIME_OUT / 1000)
		{
			GameManager::getInstance()->isWaitingRoom = 0;
			this->isWaitingEnter = false;
			this->enterFlashCount = 0;
			MessageBox("创建或进入房间超时", "错误");
		}
		else
		{
			auto result = GameManager::getInstance()->isWaitingRoom;
			//需要进入房间
			if (result == 2)
			{
				GameManager::getInstance()->isWaitingRoom = 0;
				this->isWaitingEnter = false;
				this->enterFlashCount = 0;
				Director::getInstance()->replaceScene(TransitionFadeDown::create(1.0f, DeckScene::createScene()));
			}
			else if (result == 3)
			{
				GameManager::getInstance()->isWaitingRoom = 0;
				this->isWaitingEnter = false;
				this->enterFlashCount = 0;
				MessageBox("创建或进入房间失败", "错误");
			}
			else if (result != 1)
			{
				GameManager::getInstance()->isWaitingRoom = 0;
				this->isWaitingEnter = false;
				this->enterFlashCount = 0;
			}
		}
	}
}
