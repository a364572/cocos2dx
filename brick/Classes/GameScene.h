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

	void collisionTop();
	void collisionButtom(bool base);
	void collisionLeft();
	void collisionRight();

	void changeDirection(int direction, bool base);
	void moveBall(Vec2 direction);
	CREATE_FUNC(GameScene);

private:
	int totalBrick;
	int preFlashCount;
	int curFlashCount;
	bool isStart;
	float preX;
	float horAcceration;
	float ballSpeed;
	float scale_x;
	Size visible;
	Point origin;
	vector<Vector<Sprite *>> bricks;
	Sprite *ball;
	Sprite *bottom;
	Vec2 preDirection;

};
