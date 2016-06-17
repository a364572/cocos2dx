#pragma once
#include "cocos2d.h"
#include "PokerCard.h"
USING_NS_CC;
class Player : public Sprite 
{
public:
	static Player* create(std::string);

	void setAttribute(int type, int roomPos, int index, std::vector<int> &);
	void sortCard();
	CC_SYNTHESIZE(int, type, Type);
	CC_SYNTHESIZE(int, roomPos, RoomPos);	//position in the room
	CC_SYNTHESIZE(std::string, playerName, PlayerName);
	std::vector<PokerCard *> leftCard;
	std::vector<PokerCard *> outCard;
protected:
private:
};
