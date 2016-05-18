#pragma once
#include "cocos2d.h"
USING_NS_CC;

class FruitSprite : public Sprite {

public:
	static FruitSprite* create(int row, int col, int type);
	static std::vector<std::string> nameArray;

	CC_SYNTHESIZE(int, row, Row);
	CC_SYNTHESIZE(int, col, Col);
	CC_SYNTHESIZE(int, type, Type);

};
