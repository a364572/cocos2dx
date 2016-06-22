#pragma once
#ifndef __COCOS_2D_H__
#define __COCOS_2D_H__
#include "cocos2d.h"
USING_NS_CC;
#endif

#include "cocos-ext.h"
USING_NS_CC_EXT;
class MainScene : public cocos2d::Layer
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
