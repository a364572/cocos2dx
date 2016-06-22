#include "GameRoomScene.h"

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
}

void GameRoomScene::refreshRoom()
{
	if (roomList.size() > 0)
	{
		roomList.pop_back();
	}
	roomTable->reloadData();
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

	roomList.push_back(GameRoom("room1", 2));
	roomList.push_back(GameRoom("room2", 3));
	roomList.push_back(GameRoom("room3", 2));
	roomList.push_back(GameRoom("room4", 1));
	roomList.push_back(GameRoom("room5", 1));
	roomList.push_back(GameRoom("room6", 1));
	roomList.push_back(GameRoom("room7", 1));
	roomList.push_back(GameRoom("room8", 1));
	roomList.push_back(GameRoom("room9", 1));
	roomList.push_back(GameRoom("room10", 1));
	roomList.push_back(GameRoom("room11", 1));
	roomList.push_back(GameRoom("room12", 1));
	roomList.push_back(GameRoom("room13", 1));
	roomList.push_back(GameRoom("room14", 1));
	roomList.push_back(GameRoom("room15", 1));
	roomList.push_back(GameRoom("room16", 1));
	roomList.push_back(GameRoom("room17", 1));
	roomList.push_back(GameRoom("room18", 1));
	roomList.push_back(GameRoom("room19", 1));
//	roomList.push_back(GameRoom("room20", 1));

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
//	editBox->setText("hahaha");
	addChild(editBox);

	//创建房间列表
	roomTable = TableView::create(this, Size(150, 400));
	roomTable->setDirection(ScrollView::Direction::VERTICAL);
	roomTable->ignoreAnchorPointForPosition(false);
	roomTable->setAnchorPoint(Vec2(0.5, 0.5));
	roomTable->setPosition(origin.x + visible.width / 3, origin.y + visible.height / 2);
	roomTable->setDelegate(this);
	roomTable->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	roomTable->reloadData();
	addChild(roomTable);

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
		roomList.at(cell->getIdx()).selected = true;
		if (selectIndex >= 0)
		{
			roomList.at(selectIndex).selected = false;
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
	if (idx >= roomList.size())
	{
		name = count = "";
	}
	else
	{
		name = roomList.at(idx).name;
		count = std::to_string(roomList.at(idx).count);
		isSelected = roomList.at(idx).selected;

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
		if (idx < roomList.size())
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
