#pragma once
#include "cocos2d.h"
USING_NS_CC;

//�װ峡����������
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
	Vector<Sprite *> cards;		//�洢���е���
	Sprite* progress;
	ProgressTimer* progressBar;
	Label* loadLabel;
	Size visible;
	Point origin;
};
