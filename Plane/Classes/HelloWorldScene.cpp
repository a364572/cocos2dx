#include "HelloWorldScene.h"
#include "MainScene.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//SpriteFrameCache* cache = SpriteFrameCache::sharedSpriteFrameCache();
	//cache->addSpriteFrame("bg.plist");
	//背景图片
	auto bgSprite = Sprite::create("bg.png");
	bgSprite->setAnchorPoint(Vec2(0, 0));
	bgSprite->setPosition(Vec2(origin.x, origin.y));
	addChild(bgSprite);
    
	//游戏名称放中间
	auto titleSprite = Sprite::create("title.png");
	titleSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x,
		visibleSize.height / 2 + origin.y));
	addChild(titleSprite);

	MenuItemFont *startBtn = MenuItemFont::create("Start Game", this,
		menu_selector(HelloWorld::startClicked));
	startBtn->setPosition(Vec2(visibleSize.width / 2 + origin.x,
		visibleSize.height / 3 + origin.y));
	startBtn->setFontSizeObj(20);
	startBtn->setFontNameObj("Arial");
	Menu *menu = Menu::create(startBtn, NULL);
	menu->setPosition(Vec2(0, 0));
	addChild(menu, 1);
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::startClicked(Ref* pSender) {
	Scene *scene = MainScene::createScene();
	Director::sharedDirector()->replaceScene(TransitionFadeDown::create(1, scene));
}