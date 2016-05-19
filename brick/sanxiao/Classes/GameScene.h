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
	bool judgeAll();
	int judgeInPosition(int row, int col);


	CREATE_FUNC(GameScene);

private:

	int flashCount;
	Size visible;
	Point origin;

	vector<Vector<FruitSprite *>> fruits;
};

class GameLine {
public:
	int direction;
	int length;
};
