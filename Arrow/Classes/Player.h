#pragma once
#include "cocos2d.h"
USING_NS_CC;

class Player : public Sprite {
public:
	Player();
	bool init(Vec2 pos);
	static Player* create(Vec2 pos);

	void createPlayer();
	void createPlayerHP();
	void rorateArrow(Point touch);
	void shootArrow();
	void update(float delat);

	CC_SYNTHESIZE(int, playerHP, PlayerHP);
	CC_SYNTHESIZE(bool, isRunAction, IsRunAction);
	CC_SYNTHESIZE(bool, startDraw, StartDraw);

private:
	Vec2 playerPos;
	Sprite *playerBodySprite;
	Sprite *arrowSprite;
	Sprite *hpSprite;
	ProgressTimer *hpBar;
	ccBezierConfig bezier;
	DrawNode* drawNode;
};