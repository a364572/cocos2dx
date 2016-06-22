#include "GameRoomList.h"


bool GameRoomList::init()
{
	if (Layer::init())
	{
		return true;
	}
	return false;
}

TableViewCell * GameRoomList::tableCellAtIndex(TableView * table, ssize_t idx)
{
	auto cell = table->dequeueCell();
	if (!cell)
	{
		cell = TableViewCell::create();
		auto label = Label::createWithTTF(TTFConfig("fonts/arial.ttf", 20), "Test", TextHAlignment::CENTER);
		label->ignoreAnchorPointForPosition(false);
		label->setAnchorPoint(Vec2::ZERO);
		label->setPosition(0, 0);

		cell->addChild(label);
	}
	return cell;
}
