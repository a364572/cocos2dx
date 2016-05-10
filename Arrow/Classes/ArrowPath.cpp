#include "Player.h"


static inline float bezier1(float a, float b, float c, float t) {
	return (powf(1 - t, 2) * a + 2.0f * t * (1 - t) * b + powf(t, 2) * c);
}

bool ArrowPath::initWithDuration(float t, Point ctlPoint, Point endPoint) {
	if (ActionInterval::initWithDuration(t)) {
		this->ctlPoint = ctlPoint;
		this->endPoint = endPoint;
		return true;
	}
	return false;
}
ArrowPath* ArrowPath::create(float time, Point ctlPoint, Point endPoint) {
	ArrowPath *path = new ArrowPath();
	path->initWithDuration(time, ctlPoint, endPoint);
	path->autorelease();
	return path;
}

void ArrowPath::update(float time) {
	if (_target) {
		float x = bezier1(0, ctlPoint.x, endPoint.x, time);
		float y = bezier1(0, ctlPoint.y, endPoint.y, time);

		Vec2 curPos = _target->getPosition();
		Vec2 diff = _previousPosition - curPos;
		_startPosition = _startPosition + diff;

		Vec2 newPos = _startPosition + Vec2(x, y);
		_target->setPosition(newPos);
		_previousPosition = newPos;

		if (time != 0) {
			Vec2 vec = curPos - newPos;
			float radians = vec.getAngle();
			float degree = CC_RADIANS_TO_DEGREES(-1 * radians);
			_target->setRotation(degree - 180);
		}
	}
}
