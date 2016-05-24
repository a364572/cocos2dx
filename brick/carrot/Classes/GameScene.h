#pragma once
#include "cocos2d.h"
#include "Monster.h"
#include "Tower.h"
#include "GameManager.h"
USING_NS_CC;
class GameScene : public Layer {
public:
	virtual bool init();
	virtual void update(float dt);
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	static Scene* createScene(std::string);
	static GameScene* create(std::string);

	GameScene(std::string);
	GameScene();

	void tryAddTower(Point coorPoint, Point tiledPoint);
	Point tiledToOpenglCoord(Point position);
	Point openglToTiledCoord(Point position);
	CREATE_FUNC(GameScene);
	void addTower();

	CC_SYNTHESIZE(std::string, sceneName, SceneName);
private:
	Vector<Node *> positions;
	Vector<AbstractTower *> towers;
	std::vector<std::vector<int>> mapFlag;
	TMXTiledMap* tmx;
	Layer* towerAvailiable;
	GameManager* gameManager;
	Size visible;
	Point origin;
	Point prePoint;
	float offX;
	float monsterInterval;
	int flashCount;
};

