#pragma once
#include "cocos2d.h"
#include "Player.h"
USING_NS_CC;

class GameScene : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	CREATE_FUNC(GameScene);

	Size visible;
	Point origin;
	Point prePos;
	Point curPos;

	Player* player;
	Array* monsterArr;
};
