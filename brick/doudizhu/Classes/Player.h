#pragma once
#include "cocos2d.h"
#include "PokerCard.h"
USING_NS_CC;
class Player : public Sprite 
{
public:
	static Player* create();
protected:
private:
	Vector<PokerCard *> leftCards;		//剩余的牌
	Vector<PokerCard *> outCards;		//已出的牌
	int position;						//在房间中的位置
	std::string name;					//玩家名称
};
