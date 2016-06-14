#pragma once
#include "cocos2d.h"
USING_NS_CC;
class GameManager 
{
public:
	static GameManager* instance;
	int numberOfTotalRes;
	int numberOfLoadRes;

	static GameManager* getInstance();

	void increaseCount(Ref* ref)
	{
		numberOfLoadRes++;
	}

private:
	GameManager();
	
};
