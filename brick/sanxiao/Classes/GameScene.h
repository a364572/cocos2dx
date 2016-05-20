#pragma once
#include "cocos2d.h"
#include "FruitSprite.h"
USING_NS_CC;
using namespace std;
class GameLine;
class GameScene : public Layer {
public:
	static Scene* createScene();
	virtual bool init();
	virtual void onEnterTransitionDidFinish();
	void restartGame(Ref* ref);
	void initFruit();
	void download();
	float swapPosition(FruitSprite*, FruitSprite*);
	bool judgeAll();
	int judgeInPosition(int row, int col);

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	FruitSprite *getSpriteFromPosition(Point);


	Size visible;
	Point origin;

	CREATE_FUNC(GameScene);



private:
	int reference;
	int flashCount;
	bool isDown;
	FruitSprite *preFruit;
	EventListenerTouchOneByOne *listener;

	vector<Vector<FruitSprite *>> fruits;
};