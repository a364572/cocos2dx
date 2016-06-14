#pragma once
#include "cocos2d.h"
USING_NS_CC;

//甲板场景用来出牌
class DeckScene : public Layer
{
public:
	static Scene* createScene();
	virtual bool init();

	void initDeck();
	void preload(float);
	CREATE_FUNC(DeckScene);
protected:
private:
	Vector<Sprite *> cards;		//存储所有的牌
	Sprite* progress;
	ProgressTimer* progressBar;
	Label* loadLabel;
	Size visible;
	Point origin;
};
