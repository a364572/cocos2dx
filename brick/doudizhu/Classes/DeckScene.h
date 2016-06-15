#pragma once
#include "cocos2d.h"
#include "PokerCard.h"
USING_NS_CC;

//甲板场景用来出牌
class DeckScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();

	void initDeck();
	void preload(float);
	void startGame();
	void exitGame();
	void displaySetting();
	CREATE_FUNC(DeckScene);

	CC_SYNTHESIZE(Vector<PokerCard *>, cardArray, CardArray);
protected:
private:
	Sprite* progress;
	ProgressTimer* progressBar;
	Label* loadLabel;
	Size visible;
	Point origin;
	float blackVertical;
	float blackHorizon;
};
