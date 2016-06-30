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
	enum PlayerType
	{
		ILLEGAL,
		DIZHU = 1,
		NONGMING,
	};
	static Player* create(std::string, int roomPos);

	void setAttribute(int serverPos, std::vector<int> &);
	void sortCard();
	CC_SYNTHESIZE(int, serverPos, serverPos);
	CC_SYNTHESIZE(int, roomPos, RoomPos);	//position in the room
	CC_SYNTHESIZE(PlayerType, ident, Ident);
	CC_SYNTHESIZE(std::string, playerName, PlayerName);
	CC_SYNTHESIZE(bool, ready, Ready);
	std::vector<PokerCard *> leftCard;
	std::vector<PokerCard *> outCard;
protected:
private:
};
