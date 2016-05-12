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
	void startGame(Ref* ref);
	void stopGame(Ref *ref);
	bool colllisionWithWall();
	float getRandomY(float height, float prab);
	
	CREATE_FUNC(GameScene);
private:
	int flashCount;
	int collisionIndex;
	int previousPipe;
	int score;
	float pipeBirdInterval;
	float lastTime;
	float runTime;
	float aggregation;
	float birdSpeed;
	Size visible;
	Point origin;
	Vector<Sprite *> pipeVec;
	Sprite *birdSprite;
	Label *scoreLabel;
	Menu *menu;
 };
