#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

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
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
	auto tableView = TableView::create(this, Size(100, 600));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setPosition(0, 0);
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	//addChild(tableView);
	tableView->reloadData();
	sprite = Sprite::create("Fan2.png");
	sprite->setPosition(200, 0);
	sprite->runAction(Sequence::create(DelayTime::create(1.0f), JumpBy::create(5.0f, Point(0, 600), -150, 6), nullptr));
	addChild(sprite);
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::tableCellTouched(TableView* table, TableViewCell* cell)
{
	log("Touch at index %d", cell->getIdx());
}

TableViewCell* HelloWorld::tableCellAtIndex(TableView *table, ssize_t idx)
{
	static int oldCount = 0;
	static int newCount = 0;
	auto cell = table->dequeueCell();
	if (!cell)
	{
		cell = TableViewCell::create();
		auto sprite = Sprite::create("Fan2.png");
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPosition(0, 0);
		auto label = LabelTTF::create("test", "fonts/arial.ttf", 10);
		label->setAnchorPoint(Vec2::ZERO);
		label->setPosition(0, 0);
		label->setTag(100);
		cell->addChild(label);
		cell->addChild(sprite);
		newCount++;
		log("old %d new %d", oldCount, newCount);
		return cell;
	}
	auto label = (LabelTTF *)cell->getChildByTag(100);
	label->setString(label->getString() + "1");
	oldCount++;
	log("old %d new %d", oldCount, newCount);
	return cell;
}
