#pragma once
#include "cocos2d.h"
#include "Monster.h"
#include "Tower.h"
USING_NS_CC;
class GameScene : public Layer {
public:
	virtual bool init();
	virtual void update(float dt);
	static Scene* createScene(std::string);
	static GameScene* create(std::string);
	
	GameScene(std::string);
	GameScene();
	CREATE_FUNC(GameScene);

	CC_SYNTHESIZE(std::string, sceneName, SceneName);
private:
	Vector<Node *> positions;
	Vector<Monster *> monsters;
	Vector<AbstractTower *> towers;
	TMXTiledMap* tmx;
	Size visible;
	Point origin;
	float offX;
	float monsterInterval;
	int flashCount;
};