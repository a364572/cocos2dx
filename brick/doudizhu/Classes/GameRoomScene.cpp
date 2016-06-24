#include "GameRoomScene.h"
#include "GameManager.h"

Scene * GameRoomScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameRoomScene::create();
	scene->addChild(layer);
	return scene;
}

void GameRoomScene::enterRoom()
{
}

void GameRoomScene::createRoom()
{
	std::string roomName = editBox->getText();
	if (roomName == "")
	{
		log("Input the name!");
		return;
	}
	int fd = GameManager::getInstance()->sock;
	if (fd <= 0)
	{
		return;
	}

	std::string msg = "" + MessageType::CREATE_ROOM;
	msg.append(" ");
	msg.append(roomName);
	send(fd, msg.data(), msg.size(), 0);
}

void GameRoomScene::refreshRoom()
{
}

void GameRoomScene::roomListSelected(Ref * ref, ui::ListView::EventType type)
{
	log("Selected");
	auto list = (ui::ListView *)ref;
	switch (type)
	{
	case ui::ListView::EventType::ON_SELECTED_ITEM_START:
		log("Item %d Selected", list->getCurSelectedIndex());
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


	GameManager::getInstance();

	log("Add background.png");
	auto bg = Sprite::create("background.png");
	bg->setPosition(origin.x + visible.width / 2, origin.y + visible.height / 2);
	addChild(bg, -1);
	//创建名字输入框
	editBox = EditBox::create(Size(400, 50), Scale9Sprite::create("get_time.png"));
	editBox->setPosition(Vec2(origin.x + visible.width / 2, origin.y + visible.height - 100));
	editBox->setFont("arial", 30);
	editBox->setFontColor(Color3B::ORANGE);
	editBox->setPlaceHolder("Input Your Name");
	editBox->setPlaceholderFontColor(Color3B::GRAY);
	editBox->setMaxLength(10);
	editBox->setText("");
	addChild(editBox);

	////创建房间列表
	//roomTable = TableView::create(this, Size(150, 400));
	//roomTable->setDirection(ScrollView::Direction::VERTICAL);
	//roomTable->ignoreAnchorPointForPosition(false);
	//roomTable->setAnchorPoint(Vec2(0.5, 0.5));
	//roomTable->setPosition(origin.x + visible.width / 3, origin.y + visible.height / 2);
	//roomTable->setDelegate(this);
	//roomTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//roomTable->reloadData();
	//addChild(roomTable);


	//创建listview 分别注册滑动事件和点击事件
	auto listView = ui::ListView::create();
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
	
	auto button = ui::Button::create("room.png");
	button->setName("Test Button");

	auto default_layout = ui::Layout::create();
	auto sprite = Sprite::create("room.png");
	default_layout->setSize(sprite->getContentSize());
	sprite->setPosition(Vec2(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2));
	default_layout->addChild(sprite);

	//listView->setItemModel(default_layout);
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
	//mainMenu->ignoreAnchorPointForPosition(false);
	mainMenu->setAnchorPoint(Vec2(0.5, 0.5));
	mainMenu->setPosition(origin.x + visible.width * 2 / 3, origin.y + visible.height / 2);
	addChild(mainMenu);
	return true;
}

void GameRoomScene::tableCellTouched(TableView * table, TableViewCell * cell)
{
	log("Cell touch %d", cell->getIdx());
	if (cell->getIdx() == selectIndex)
	{
		return;
	}
	//背景变色
	auto bg = (Sprite *)(cell->getChildByTag(ROOM_BACKGROUND));
	if (bg)
	{
		bg->setColor(Color3B(255, 255, 0));
		GameManager::getInstance()->roomList.at(cell->getIdx()).selected = true;
		if (selectIndex >= 0)
		{
			GameManager::getInstance()->roomList.at(selectIndex).selected = false;
			if (table->cellAtIndex(selectIndex))
			{
				auto selectBG = (Sprite *)table->cellAtIndex(selectIndex)->getChildByTag(ROOM_BACKGROUND);
				if (selectBG)
				{
					selectBG->setColor(Color3B(255, 255, 255));

				}
			}
		}
		selectIndex = cell->getIdx();
	}
}

TableViewCell * GameRoomScene::tableCellAtIndex(TableView * table, ssize_t idx)
{
	std::string name;
	std::string count;
	bool isSelected = false;
	auto cell = table->dequeueCell();
	if (idx >= GameManager::getInstance()->roomList.size())
	{
		name = count = "";
	}
	else
	{
		name = GameManager::getInstance()->roomList.at(idx).name;
		count = std::to_string(GameManager::getInstance()->roomList.at(idx).count);
		isSelected = GameManager::getInstance()->roomList.at(idx).selected;

	}
	if (!cell)
	{

		cell = TableViewCell::create();

		auto bg = Sprite::create("room.png");
		bg->setTag(ROOM_BACKGROUND);
		bg->setAnchorPoint(Vec2::ZERO);
		bg->setPosition(0, 0);
		if (isSelected)
		{
			bg->setColor(Color3B(255, 255, 0));
		}
		else
		{
			bg->setColor(Color3B(255, 255, 255));
		}
		cell->addChild(bg, -1);

		auto nameLabel = Label::createWithTTF(TTFConfig("fonts/arial.ttf", 20), name, TextHAlignment::CENTER);
		nameLabel->ignoreAnchorPointForPosition(false);
		nameLabel->setAnchorPoint(Vec2::ZERO);
		nameLabel->setPosition(0, 0);
		nameLabel->setTag(ROOM_NAME);
		nameLabel->setTextColor(Color4B::BLACK);
		cell->addChild(nameLabel, 1);

		auto countLabel = Label::createWithTTF(TTFConfig("fonts/arial.ttf", 20), count, TextHAlignment::CENTER);
		countLabel->ignoreAnchorPointForPosition(false);
		countLabel->setAnchorPoint(Vec2(0, 0));
		countLabel->setPosition(100, 0);
		countLabel->setTag(NUMBER_OF_PLAYERS);
		countLabel->setTextColor(Color4B::BLACK);
		cell->addChild(countLabel, 1);
	}
	else
	{
		if (idx < GameManager::getInstance()->roomList.size())
		{
			auto nameLabel = (Label*)(cell->getChildByTag(ROOM_NAME));
			auto countLabel = (Label*)(cell->getChildByTag(NUMBER_OF_PLAYERS));
			auto bg = (Sprite *)(cell->getChildByTag(ROOM_BACKGROUND));
			nameLabel->setString(name);
			countLabel->setString(count);
			if (isSelected)
			{
				bg->setColor(Color3B(255, 255, 0));
			}
			else
			{
				bg->setColor(Color3B(255, 255, 255));
			}
		}
	}
	return cell;
}
