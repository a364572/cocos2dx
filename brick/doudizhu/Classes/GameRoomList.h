#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC; 
USING_NS_CC_EXT;
class GameRoomList : public Layer, public TableViewDataSource, public TableViewDelegate
{
public:
	virtual bool init();
	virtual void scrollViewDidScroll(ScrollView* view) {};
	virtual void scrollViewDidZoom(ScrollView* view) {};
	virtual void tableCellTouched(TableView* table, TableViewCell* cell) {};
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx) { return Size(300, 50); };
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table) { return 20; };

	CREATE_FUNC(GameRoomList);
};
