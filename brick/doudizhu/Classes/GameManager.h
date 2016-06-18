#pragma once
#include "cocos2d.h"
#include "PokerCard.h"
USING_NS_CC;

class GameManager 
{
public:
	static GameManager* getInstance();
	
	void initCard(Ref* ref);
	void initNumbers(Ref* ref);
	void increaseCount(Ref* ref);

	static GameManager* instance;
	int numberOfTotalRes;
	int numberOfLoadRes;
	Vector<PokerCard *> rawCardArray;
	Vector<Sprite *> numberArray;
	/** store all the judge function by their priority **/
private:
	GameManager();
	
}; 