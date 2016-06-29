#pragma once
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC_EXT;

class GameRoomScene : public Layer
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
	void refreshListView();
	
	void roomListSelected(Ref* ref, ui::ListView::EventType type);
	void roomListScrolled(Ref* ref, ui::ScrollView::EventType type);

	virtual bool init();
	virtual void update(float delta);

	int selectIndex;
	int listFlashCount;
	int enterFlashCount;
	bool isWaitingList;
	bool isWaitingEnter;
	Color3B selectColor;
	Size visible;
	Point origin;
	TableView* roomTable;
	Menu* mainMenu;
	ui::ListView* listView;
};
