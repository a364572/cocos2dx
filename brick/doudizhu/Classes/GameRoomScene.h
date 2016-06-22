#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class GameRoom
{
public:
	GameRoom() {}
	GameRoom(std::string name, int count) { this->name = name; this->count = count; selected = false; };
	std::string name;
	int count;
	bool selected;
};

class GameRoomScene : public Layer, public TableViewDataSource, public TableViewDelegate,
	public EditBoxDelegate
{
public:
	
	enum LabelTag
	{
		ROOM_BACKGROUND = 100,
		ROOM_NAME,
		NUMBER_OF_PLAYERS,
	};

	CREATE_FUNC(GameRoomScene);
	static Scene* createScene();

	void enterRoom();
	void createRoom();
	void refreshRoom();

	virtual bool init();
	virtual void editBoxEditingDidBegin(EditBox* edit) { log("BEGIN"); };
	virtual void editBoxEditingDidEnd(EditBox* edit) { log("END"); };
	virtual void editBoxTextChanged(EditBox* edit) { log("CHANGE"); };
	virtual void editBoxReturn(EditBox* edit) { log("RETURN"); };
	virtual void scrollViewDidScroll(ScrollView* view) {};
	virtual void scrollViewDidZoom(ScrollView* view) {};
	virtual void tableCellTouched(TableView* table, TableViewCell* cell);
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx) { return Size(150, 50); };
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
	virtual ssize_t numberOfCellsInTableView(TableView *table) { return 20; };


	int selectIndex;
	Color3B selectColor;
	Size visible;
	Point origin;
	std::vector<GameRoom> roomList;
	TableView* roomTable;
	EditBox* editBox;
	Menu* mainMenu;
};
