#pragma once
#include "cocos2d.h"
#include <vector>
USING_NS_CC;
using namespace std;

class GameScene : public Layer {
public :
	virtual bool init();
	static Scene* createScene();
	void initGame();

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	CREATE_FUNC(GameScene);

private:
	bool isStart;
	float preX;
	Size visible;
	Point origin;
	vector<Vector<Sprite *>> bricks;
	Sprite *ball;
	Sprite *bottom;

};
