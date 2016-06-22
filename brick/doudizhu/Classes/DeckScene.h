#pragma once
#include "Player.h"
#ifndef __COCOS_2D_H__
#define __COCOS_2D_H__
#include "cocos2d.h"
USING_NS_CC;
#endif

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
	void flashCard();
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchesMoved(const std::vector<Touch *>& touches, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
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
	
	std::vector<int> randCards;	//card sorted randomly

	Sprite* pokerBack1;		//back card1
	Sprite* pokerBack2;		//back card2
	Sprite* pokerBack3;		//back card3
	Sprite* pokerBack4;		//back card4
	Sprite* pokerBack5;		//back card5

	Sprite* bottomCard1;	
	Sprite* bottomCard2;	
	Sprite* bottomCard3;

	PokerArrayType currentPokerType;			//the poker type in this round

	const float shuffleTime = 3.0f;
	const float cardIntervalHorizontal = 30.0f;
	const float cardIntervalVertical= 30.0f;
};
