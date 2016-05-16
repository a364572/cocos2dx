#pragma once
#include "cocos2d.h"
USING_NS_CC;

class MenuScene : public Layer {
public:
	virtual bool init();
	static Scene* createScene();
	void startGame(Ref *);
	void endGame(Ref *);

	CREATE_FUNC(MenuScene);
private:
	Size visible;
	Point origin;
};
