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

	bool isSelect()
	{
		return select;
	}
	void setSelect(bool select)
	{
		this->select = select;
	}
	bool isQueen() 
	{
		if (this->getValueInAll() >= 52)
		{
			return true;
		}
		return false;
	}
	CC_SYNTHESIZE(int, type, Type);					//花色	
	CC_SYNTHESIZE(int, valueInType, ValueInType);	//在花色内部的值
	CC_SYNTHESIZE(int, valueInAll, ValueInAll);		//所有牌中的值
protected:
private:
	bool select;
};

typedef bool(*JUDGE_FUNCTION)();
class PokerArray
{
public:
	enum PokerArrayType
	{
		ILLEGAL,			//不合法

		SINGLE,				//单牌

		PAIR,				//对子

		THREE_WITH_NONE,	//三个

		THREE_WITH_SINGLE,	//三带一
		BOMB,				//炸弹

		THREE_WITH_PAIR,	//三带一对
		SEQUENCE,			//顺子

		BOMB_WITH_SINGLE,	//炸弹带两个
		MULTIPLE_PAIR,		//连对
		PLANE_WITH_NONE,	//飞机不带

		BOMB_WITH_PAIR,		//炸弹带两对
		PLANE_WITH_SINGLE,	//飞机带单牌

		PLANE_WITH_PAIR,	//飞机带对子

		QUEEN_BOMB,			//王炸
	};

	//all the cards should be sort by their value in type firstly
	static bool isSingle(PokerArray *);
	static bool isPair(PokerArray *);
	static bool isSequence(PokerArray *);
	static bool isMultiplePair(PokerArray *);
	static bool isThreeWithNone(PokerArray *);
	static bool isThreeWithSingle(PokerArray *);
	static bool isThreeWithPair(PokerArray *);
	static bool isPlaneWithNone(PokerArray *);
	static bool isPlaneWithSingle(PokerArray *);
	static bool isPlaneWithPair(PokerArray *);
	static bool isBomb(PokerArray *);
	static bool isBombWithSingle(PokerArray *);
	static bool isBombWithPair(PokerArray *);
	static bool isQueenBomb(PokerArray *);

	/** sort the card by the length of each value **/
	void sortByLength();
	void sortByValue();

	/** sort the card based on the value, 
	cards of the same value will be further sorted by the type **/
	static int compareByLocalValue(PokerCard *card1, PokerCard *card2)
	{
		if (card2->getValueInType() == card1->getValueInType())
		{
			return card2->getType() < card1->getType();
		}
		return card2->getValueInType() < card1->getValueInType();
	}
	PokerArray()
	{
		type = ILLEGAL;
	}
	PokerArray(std::vector<PokerCard *> cards)
	{
		this->type = ILLEGAL;
		for (auto card : cards)
		{
			cardArray.push_back(card);
		}
		bool (*pFunc)() = nullptr;
		pFunc = isQueenBomb;
		std::vector<JUDGE_FUNCTION> funList;
		funList->push_back(isQueenBomb);
		for (int i = QUEEN_BOMB; i >= SINGLE; i--)
		{
		}
	}
	std::vector<PokerCard *> cardArray;
	PokerArrayType type;
	int lengthOfType;
};