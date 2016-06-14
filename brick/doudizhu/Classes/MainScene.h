#pragma once
#include "cocos2d.h"
USING_NS_CC;

class MainScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(MainScene);

	void startGame();
	void displayHelp();
	void exitGame();
	void backFromHelp();
protected:
private:
	Size visible;
	Point origin;
	Sprite* helpSprite;
	Menu* mainMenu;
};
