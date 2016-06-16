#pragma once
#include "cocos2d.h"
#include "PokerCard.h"
#include "Player.h"
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
	void outCard(int flag);
	void callDiZhu(int flag);
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
	Menu* readyMenu;
	Menu* callMenu;
	Menu* robMenu;
	Menu* outCardMenu;
	Player* player;			//myself
	Player* player1;		//player in the left
	Player* player2;		//player in the right
	bool isInGame;			//whether the game is running
};
