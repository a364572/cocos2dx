#pragma once
#include "cocos2d.h"
USING_NS_CC;
class AirPlane : public Sprite {
public:
	static AirPlane* create(char *name);
private:
	virtual bool onTouchBegan(Point *point, Event* event);
	virtual void onTouchMoved(Point *, Event*);
	virtual void onTouchEnded(Point *, Event*);

	Point curPoint;
};
