#pragma once
#include "cocos2d.h"
USING_NS_CC;
class PokerCard : public Sprite
{
public:
	enum PokerType
	{
		FANGKUAI = 1,
		MEIHUA,
		HONGXIN,
		HEITAO,
		DAXIAOWANG
	};

	static PokerCard* create(SpriteFrame *);

	CC_SYNTHESIZE(int, type, Type);
	CC_SYNTHESIZE(int, valueInType, ValueInType);
	CC_SYNTHESIZE(int, valueInAll, ValueInAll);
protected:
private:
	
};
