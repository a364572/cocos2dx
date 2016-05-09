#include "MainScene.h"
#include "EnemyPlane.h"
bool MainScene::init() {

	if (!Layer::init()) {
		return false;
	}

	initBgLayer();
	initUILayer();
	initLogicLayer();
	return true;
}

Scene* MainScene::createScene() {
	auto scene = Scene::create();
	auto layer = MainScene::create();

	scene->addChild(layer);
	return scene;
}

void MainScene::initBgLayer() {
	auto visibleSize = Director::sharedDirector()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	bgLayer = Layer::create();
	bgLayer->setContentSize(Size(visibleSize.width, visibleSize.height));
	bgLayer->setPosition(0, 0);

	//背景图
	bgSprite1 = Sprite::create("bg.png");
	bgSprite1->setAnchorPoint(Vec2(0, 0));
	bgSprite1->setPosition(Vec2(0, 0));
	bgLayer->addChild(bgSprite1, 1);

	//两张背景图，用于滚动背景
	bgSprite2 = Sprite::create("bg.png");
	bgSprite2->setAnchorPoint(Vec2(0, 0));
	bgSprite2->setPosition(Vec2(0, visibleSize.height - 1));
	bgLayer->addChild(bgSprite2, 1);

	//左上角暂停按钮
	pauseMenuItem = MenuItemImage::create("pause.png", "continue.png",
		this, menu_selector(MainScene::pauseGame));
	pauseMenuItem->setAnchorPoint(Vec2(0, 1));
	pauseMenuItem->setPosition(Vec2(origin.x, visibleSize.height + origin.y));

	//左上角分数
	scoreMenuFont = MenuItemFont::create("    Score:00000");
	scoreMenuFont->setAnchorPoint(Vec2(0, 1));
	scoreMenuFont->setPosition(Vec2(origin.x + pauseMenuItem->getContentSize().width,
		visibleSize.height + origin.y));
	scoreMenuFont->setFontSizeObj(10);
	scoreMenuFont->setFontNameObj("Arial");
	
	auto menu = Menu::create(pauseMenuItem, NULL);
	menu->addChild(scoreMenuFont);
	menu->setAnchorPoint(Vec2(0, 0));
	menu->setPosition(Vec2(0, 0));
	bgLayer->addChild(menu, 2);

	isPause = true;
	bgHeight = 0;
	score = 0;
	addChild(bgLayer, 1);
}

void MainScene::initUILayer() {
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto visible = Director::getInstance()->getVisibleSize();
	auto layer = LayerColor::create(Color4B(0, 0, 0, 150));

	//开始,重新开始和退出按钮
	auto startBtn = MenuItemFont::create("Start", this, menu_selector(MainScene::startGame));
	auto restartBtn = MenuItemFont::create("Restart", this, menu_selector(MainScene::restartGame));
	auto exitBtn = MenuItemFont::create("Exit", this, menu_selector(MainScene::exitGame));

	startBtn->setAnchorPoint(Vec2(0.5, 0.5));
	startBtn->setPosition(origin.x + visible.width / 2, origin.y + visible.height / 3 * 2);
	restartBtn->setAnchorPoint(Vec2(0.5, 0.5));
	restartBtn->setPosition(origin.x + visible.width / 2, origin.y + visible.height / 2);
	restartBtn->setVisible(false);
	exitBtn->setAnchorPoint(Vec2(0.5, 0.5));
	exitBtn->setPosition(origin.x + visible.width / 2, origin.y + visible.height / 3);

	mainMenu = Menu::create();
	mainMenu->addChild(startBtn);
	mainMenu->addChild(restartBtn);
	mainMenu->addChild(exitBtn);

	mainMenu->setPosition(Vec2(0, 0));
	layer->addChild(mainMenu);
	addChild(layer, 2);
}

void MainScene::initLogicLayer() {
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto visible = Director::getInstance()->getVisibleSize();

	logicalLayer = LayerColor::create(Color4B(0, 0, 0, 0));
	logicalLayer->setPosition(Vec2(0, 0));

	//创建飞机
	planeSprite = Sprite::create("plane.jpg");
	planeSprite->setAnchorPoint(Vec2(0.5, 0));
	planeSprite->setPosition(Vec2(origin.x + visible.width / 2, origin.y));
	planeSprite->setVisible(false);
	logicalLayer->addChild(planeSprite, 2);

	//创建多个子弹
	for (int i = 0; i < BULLET_SIZE; i++) {
		bullet[i] = Sprite::create("bullet.png");
		bullet[i]->setAnchorPoint(Vec2(0.5, 1));
		bullet[i]->setPosition(Vec2(origin.x + visible.width / 2, origin.y - BULLET_INTERVAL * i));
		logicalLayer->addChild(bullet[i], 1);
	}

	//设置监听器，拖动飞机
	auto dispatch = Director::getInstance()->getEventDispatcher();
	auto listen = EventListenerTouchOneByOne::create();
	listen->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
	listen->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);
	listen->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);
	dispatch->addEventListenerWithSceneGraphPriority(listen, this);
	
	srand(time(NULL));
	enemyArr = Array::create();
	enemyArr->retain();
	enemyArr->count();
	flashCount = 0;
	addChild(logicalLayer, 3);

}

void MainScene::rollBg() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	bgHeight++;
	bgSprite1->setPosition(Vec2(0, 0 - bgHeight));
	bgSprite2->setPosition(Vec2(0, bgSprite1->getContentSize().height - 1 - bgHeight));

	if (bgHeight >= bgSprite1->getContentSize().height) {
		bgHeight = 0;
	}
}

void MainScene::pauseGame(Ref * ref) {

	if (isPause)
	{	isPause = false;
		scheduleUpdate();
		mainMenu->setVisible(false);
	}
	else {
		isPause = true;
		mainMenu->setVisible(true);
		unscheduleUpdate();
		((MenuItemFont *)((mainMenu->getChildren()).at(0)))->setString("Continue");
	}
}

void MainScene::startGame(Ref* ref) {
	if (isPause) {
		scheduleUpdate();
		isPause = false;
		planeSprite->setVisible(true);
		mainMenu->setVisible(false);
		((MenuItemFont *)((mainMenu->getChildren()).at(0)))->setString("Continue");
		((MenuItemFont *)((mainMenu->getChildren()).at(1)))->setVisible(true);
	}
}

void MainScene::restartGame(Ref* ref) {
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto visible = Director::getInstance()->getVisibleSize();
	planeSprite->setPosition(Vec2(origin.x + visible.width / 2, origin.y));
	planeSprite->setVisible(true);

	for (int i = 0; i < BULLET_SIZE; i++) {
		bullet[i]->setPosition(Vec2(origin.x + visible.width / 2, origin.y - BULLET_INTERVAL * i));
	}
	mainMenu->setVisible(false);
	((MenuItemFont *)((mainMenu->getChildren()).at(0)))->setString("Start");
	scheduleUpdate();
	isPause = false;
}
void MainScene::exitGame(Ref* ref) {
	cleanAll();
	Director::getInstance()->end();
	exit(0);
}

void MainScene::update(float delat) {
	if (!isPause) {
		rollBg();
		launchBullet();
		createEnemy();
		flashEnemy();
	}
}

bool MainScene::onTouchBegan(Touch *touch, Event *unused_event) {
	if (isPause) {
		return false;
	}
	curPoint = touch->getLocation();
	return true;
}
void MainScene::onTouchMoved(Touch *touch, Event *unused_event) {
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto visible = Director::getInstance()->getVisibleSize();

	auto diff = touch->getLocation().x - curPoint.x;
	if (planeSprite->getPositionX() + diff >= origin.x + visible.width) {
		planeSprite->setPosition(Vec2(origin.x + visible.width, origin.y));
	}
	else if (planeSprite->getPositionX() + diff <= origin.x) {
		planeSprite->setPosition(Vec2(origin.x, origin.y));
	}
	else {
		planeSprite->setPosition(Vec2(diff + planeSprite->getPositionX(), origin.y));
	}
	//char tmp[128];
	//sprintf(tmp, "Oringin:%f, %f  Visible:%f, %f  Touch:%f, %f Position:%f, %f", origin.x, origin.y,
	//	visible.width, visible.height, touch->getLocation().x, touch->getLocation().y,
	//	diff + planeSprite->getPosition().x, planeSprite->getPosition().y);
	//log(tmp);
	curPoint = touch->getLocation();
}
void MainScene::onTouchEnded(Touch *touch, Event *unused_event) {
	onTouchMoved(touch, unused_event);
}

void MainScene::launchBullet() {
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto visible = Director::getInstance()->getVisibleSize();

	for (int i = 0; i < BULLET_SIZE; i++) {
		float x = bullet[i]->getPositionX();
		if (bullet[i]->getPositionY() <= planeSprite->getPositionY()) {
			x = planeSprite->getPositionX();
		}
		bullet[i]->setPosition(Vec2(x, bullet[i]->getPositionY() + 5));
		if (bullet[i]->getPositionY() >= origin.y + visible.height + bullet[i]->getContentSize().height) {
			bullet[i]->setPosition(Vec2(planeSprite->getPositionX(), origin.y));
		}
		if (bullet[i]->getPositionY() <= origin.y) {
			bullet[i]->setVisible(true);
		}
	}
}

void MainScene::createEnemy() {
	auto visible = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	if (++flashCount < ENEMY_INTERVAL) {
		return;
	}
	flashCount = 0;
	enum ENEMY_TYPE type = SMALL;
	smallCount++;
	if (smallCount >= 10 && bigCount <= 0) {
		bigCount = 1;
		smallCount = 0;
		type = BIG;
	}
	else if(smallCount >= 5 && mediumCount <= 0)
	{
		mediumCount = 1;
		type = MEDIUM;
	}
	auto enemy = EnemyPlane::createEnemy(type);

	auto x = rand() % (int)(visible.width) + origin.x;
	enemy->setPosition(Vec2(x, origin.y + visible.height + enemy->getContentSize().height));
	enemyArr->addObject(enemy);

	logicalLayer->addChild(enemy);
}

void MainScene::flashEnemy() {
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto visible = Director::getInstance()->getVisibleSize();

	for (int i = 0; i < enemyArr->count(); i++) {
		EnemyPlane *enemy = (EnemyPlane *)enemyArr->getObjectAtIndex(i);
		enemy->setPosition(Vec2(enemy->getPositionX(), enemy->getPositionY() - ENEMY_SPEED));
		if (enemy->boundingBox().intersectsRect(planeSprite->boundingBox())) {
			cleanAll();
			return;
		}

		if (enemy->getPositionY() <= origin.y) {
			if (enemy->type == MEDIUM) {
				mediumCount--;
			}
			else if (enemy->type == BIG) {
				bigCount--;
			}
			enemyArr->removeObjectAtIndex(i);
			enemy->removeFromParentAndCleanup(true);
			i--;
			continue;
		}
		
		for (int j = 0; j < BULLET_SIZE; j++) {
			if (bullet[j]->isVisible() && enemy->boundingBox().intersectsRect(bullet[j]->boundingBox())) {
				enemy->blood = enemy->blood - 1;
				if (enemy->blood <= 0) {
					score += enemy->score;
					char tmp[32];
					sprintf(tmp, "    score:%05d", score);
					scoreMenuFont->setString(tmp);
					if (enemy->type == MEDIUM) {
						mediumCount--;
					}
					else if(enemy->type == BIG){
						bigCount--;
					}
					enemyArr->removeObjectAtIndex(i);
					enemy->removeFromParentAndCleanup(true);
					i--;
					break;
				}
				bullet[j]->setVisible(false);
			}
		}
	}
}

void MainScene::cleanAll() {
	unscheduleUpdate();
	isPause = true;
	mainMenu->setVisible(true);
	char tmp[16];
	sprintf(tmp, "Score:%d", score);
	((MenuItemFont *)((mainMenu->getChildren()).at(0)))->setString(tmp);
	for (int i = 0; i < enemyArr->count(); i++) {
		EnemyPlane *enemy = (EnemyPlane *)(enemyArr->getObjectAtIndex(i));
		enemy->removeFromParentAndCleanup(true);
		enemyArr->removeObjectAtIndex(i);
		i--;
	}

	auto origin = Director::getInstance()->getVisibleOrigin();
	auto visible = Director::getInstance()->getVisibleSize();
	planeSprite->setPosition(Vec2(origin.x + visible.width / 2, origin.y));
	planeSprite->setVisible(false);

	for (int i = 0; i < BULLET_SIZE; i++) {
		bullet[i]->setPosition(Vec2(origin.x + visible.width / 2, origin.y - BULLET_INTERVAL * i));
	}
	score = flashCount = smallCount = mediumCount = bigCount = 0;
	scoreMenuFont->setString("    score:00000");
}