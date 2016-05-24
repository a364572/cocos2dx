#pragma once
#include "cocos2d.h"
USING_NS_CC;
class Monster : public Sprite {
public:
	static Monster *create(std::string name, float speed, float blood, Vector<Node *>);
	void forwad();
	float getRunTime();

	CC_SYNTHESIZE(float, speed, Speed);
	CC_SYNTHESIZE(float, blood, Blood);
	CC_SYNTHESIZE(int, currentIndex, CurrentIndex);
	CC_SYNTHESIZE(bool, isDead, IsDead);
private:
	Vector<Node *> path;
};