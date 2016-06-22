#pragma once
#include "PokerCard.h"
#ifndef __COCOS_2D_H__
#define __COCOS_2D_H__
#include "cocos2d.h"
USING_NS_CC;
#endif
class Player : public Sprite 
{
public:
	static Player* create(std::string);

	void setAttribute(int type, int roomPos, int index, bool dizhu, std::vector<int> &);
	void sortCard();
	CC_SYNTHESIZE(int, type, Type);
	CC_SYNTHESIZE(int, roomPos, RoomPos);	//position in the room
	CC_SYNTHESIZE(std::string, playerName, PlayerName);
	CC_SYNTHESIZE(bool, diZhu, DiZhu);
	std::vector<PokerCard *> leftCard;
	std::vector<PokerCard *> outCard;
protected:
private:
};
