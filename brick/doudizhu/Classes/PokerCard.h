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
	CC_SYNTHESIZE(int, type, Type);					//��ɫ	
	CC_SYNTHESIZE(int, valueInType, ValueInType);	//�ڻ�ɫ�ڲ���ֵ
	CC_SYNTHESIZE(int, valueInAll, ValueInAll);		//�������е�ֵ
protected:
private:
	bool select;
};

enum PokerArrayType
{
	ILLEGAL,			//���Ϸ�					0
	SINGLE,				//����			== 1		1
	QUEEN_BOMB,			//��ը			== 2		2
	PAIR,				//����			== 2		3
	THREE_WITH_NONE,	//����			== 3		4
	BOMB,				//ը��			== 4		5
	THREE_WITH_SINGLE,	//����һ		== 4		6
	THREE_WITH_PAIR,	//����һ��		== 5		7
	SEQUENCE,			//˳��			>= 5		8
	BOMB_WITH_SINGLE,	//ը��������	== 6		9
	MULTIPLE_PAIR,		//����			>= 6		10
	PLANE_WITH_NONE,	//�ɻ�����		>= 6		11
	BOMB_WITH_PAIR,		//ը��������	== 8		12
	PLANE_WITH_SINGLE,	//�ɻ�������	>= 8		13
	PLANE_WITH_PAIR,	//�ɻ�������	>= 10		14
};

class PokerArray
{
public:

	/** sort the card by the length of each value **/
	void sortByLength();
	void sortByValue();

	PokerArray();
	PokerArray(std::vector<PokerCard *> cards);

	/** store all the card **/
	std::vector<PokerCard *> cardArray;
	PokerArrayType type;
	int lengthOfType;
};

typedef bool(*JUDGE_FUNCTION)(PokerArray *);
class PokerArrayUtil
{
public:
	/** sort the card based on the value,
	cards of the same value will be further sorted by the type **/
	static bool compareByLocalValue(PokerCard *card1, PokerCard *card2);

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

	static void judgeTypeOfCards(PokerArray *);
	static void initFunction();

	static std::map<PokerArrayType, JUDGE_FUNCTION> funMap;	//easy to get the counterpart function
	static std::vector<PokerArrayType> funList;				//card type priority

private:
	static PokerArrayUtil* instance;
};