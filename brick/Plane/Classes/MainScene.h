#pragma once
#include "cocos2d.h"
USING_NS_CC;

#define BULLET_SIZE 5
#define BULLET_INTERVAL 70
#define ENEMY_INTERVAL 20
#define ENEMY_SPEED 3
class MainScene : public Layer {
public:
	static Scene* createScene();
	bool init();
	void update(float delat);
	CREATE_FUNC(MainScene);

	void pauseGame(Ref *);
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	//void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	//void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
private:
	LayerColor* uiLayer;
	Layer* bgLayer;
	Layer* logicLayer;

	void initUILayer();
	void initBgLayer();
	void initLogicLayer();
	void rollBg();
	void launchBullet();
	void createEnemy();
	void flashEnemy();
	void cleanAll();

	void startGame(Ref*);
	void restartGame(Ref*);
	void exitGame(Ref*);

	int bgHeight;
	Sprite* bgSprite1, *bgSprite2;

	MenuItemImage *pauseMenuItem;
	MenuItemFont *scoreMenuFont;
	Menu *mainMenu;
	SpriteFrame* pauseImageSprite;
	SpriteFrame* continueImageSprite;
	Sprite* planeSprite;
	Sprite* bullet[BULLET_SIZE];
	Array* enemyArr;
	Point curPoint;
	bool isPause;
	int score;
	int flashCount;
	int bigCount;
	int mediumCount;
	int smallCount;
	LayerColor *logicalLayer;
};
