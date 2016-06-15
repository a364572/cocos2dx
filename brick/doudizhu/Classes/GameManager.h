#pragma once
#include "cocos2d.h"
#include "PokerCard.h"
USING_NS_CC;
class GameManager 
{
public:
	static GameManager* instance;
	int numberOfTotalRes;
	int numberOfLoadRes;

	static GameManager* getInstance();

	void initCard(Ref* ref);
	void initNumbers(Ref* ref);
	void increaseCount(Ref* ref);

	CC_SYNTHESIZE(Vector<PokerCard *>, rawCardArray, RawCardArray);
	CC_SYNTHESIZE(Vector<Sprite *>, numberArray, NumberArray);
private:
	GameManager();
	
}; 