#pragma once
#include "cocos2d.h"
USING_NS_CC;
class GameScene : public Layer{
public :
	virtual bool init();
	Scene *createScene();
	const static float PIPE_INTERVAL;
	CREATE_FUNC(GameScene);
private:
	Size visible;
	Point origin;
 };
