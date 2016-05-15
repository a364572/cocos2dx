#pragma once
#include "cocos2d.h"
USING_NS_CC;
class MainScene : public Layer {
public:
	virtual bool init();
	static Scene* createScene();

	void startGame(Ref *ref);
	void exitGame(Ref *ref);

	CREATE_FUNC(MainScene);

private:
	Menu *mainMenu;
	MenuItemLabel* startBtn;
	MenuItemLabel* exitBtn;

	Size visible;
	Point origin;
};
