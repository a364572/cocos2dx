#pragma once
#include "cocos2d.h"
USING_NS_CC;
#define SPEED 50
class GameScene : public Layer{
public :
	virtual bool init();
	static Scene *createScene();
	float PIPE_INTERVAL;
	void update(float time);
	void initVariable();
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	float getRandomY(float height, float prab);
	
	CREATE_FUNC(GameScene);
private:
	int flashCount;
	int collisionIndex;
	float pipeBirdInterval;
	float lastTime;
	float runTime;
	float aggregation;
	float birdSpeed;
	Size visible;
	Point origin;
	Vector<Sprite *> pipeVec;
	Sprite *birdSprite;
 };
