#include "Plane.h"

void AirPlane::onTouchMoved(Point *point, Event* event) {
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto visible = Director::getInstance()->getVisibleSize();

	auto diff = point->x - curPoint.x;
	if (getPositionX() + diff >= origin.x + visible.width) {
		setPosition(Vec2(origin.x + visible.width, origin.y));
	}
	else if (getPositionX() + diff <= origin.x) {
		setPosition(Vec2(origin.x, origin.y));
	}
	else {
		setPosition((Vec2(origin.x + (diff + getPositionX()), origin.y));
	}
	curPoint = *point;
}
bool AirPlane::onTouchBegan(Point *point, Event* event) {
	curPoint = *point;
}
void AirPlane::onTouchEnded(Point *point, Event* event) {

}

AirPlane* AirPlane::create(char *name) {
	auto visible = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	AirPlane *plane = new AirPlane();
	if (plane && plane->initWithFile("")) {
		plane->autorelease();
		plane->setAnchorPoint(Vec2(0.5, 0));
		plane->setPosition(Vec2(origin.x + visible.width/ 2, origin.y));
		return plane;
	}
	return NULL;
}