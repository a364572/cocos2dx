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
	virtual void update(float time);

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	void changeDirection(bool);
	void moveBall(Vec2 direction);
	CREATE_FUNC(GameScene);

private:
	int totalBrick;
	bool isStart;
	float preX;
	Size visible;
	Point origin;
	vector<Vector<Sprite *>> bricks;
	Sprite *ball;
	Sprite *bottom;
	Vec2 preDirection;

};
