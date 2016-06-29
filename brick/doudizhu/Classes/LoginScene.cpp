#include "LoginScene.h"
#include "GameManager.h"
#include "GameRoomScene.h"

bool LoginScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visible = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("background.png");
	bg->setPosition(visible.width / 2 + origin.x, visible.height / 2 + origin.y);
	addChild(bg);

	this->inputText = TextFieldTTF::textFieldWithPlaceHolder("Input name", "arial", 25);
	inputText->setPosition(Point(origin.x + visible.width / 2, origin.y + visible.height * 3 / 4));
	addChild(inputText);

	//��¼��ť ���֮����ע��
	loginBtn = Sprite::create("blue_btn_ok.png");
	loginBtn ->setPosition(inputText->getPositionX() + 200, inputText->getPositionY());
	addChild(loginBtn);

	auto listen = EventListenerTouchOneByOne::create();
	listen->onTouchBegan = CC_CALLBACK_2(LoginScene::onTouchBegan, this);
	listen->onTouchMoved = CC_CALLBACK_2(LoginScene::onTouchMoved, this);
	listen->onTouchEnded = CC_CALLBACK_2(LoginScene::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listen, this);

	return true;
}

Scene * LoginScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoginScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoginScene::onTouchBegan(Touch * touch, Event * event)
{
	if (GameManager::getInstance()->isConnected)
	{
		return false;
	}
	//������������
	if (inputText->getBoundingBox().containsPoint(touch->getLocation()))
	{
		inputText->attachWithIME();
	}
	else
	{
		inputText->detachWithIME();
		//������ǵ�¼��ť
		if (loginBtn->getBoundingBox().containsPoint(touch->getLocation()) && !isConnecting)
		{
			auto name = inputText->getString();
			if (name.size() == 0)
			{
				MessageBox("����������", "Warning");
				return false;
			}
			if (name.find(' ') != std::string::npos ||
				name.find('\n') != std::string::npos || 
				name.find("\r\n") != std::string::npos) 
			{
				MessageBox("���Ʋ��ܰ��������ַ�", "Warning");
				return false;
			}
			checkCount = 0;
			isConnecting = true;

			//������������ ͬʱ������ʱ���������״̬ �ɹ�֮���л�����
			auto manager = GameManager::getInstance();
			manager->player = name;
			manager->connectWithServer();
			schedule(schedule_selector(LoginScene::checkStatus), SCHEDULE_INTERVAL / 1000.0f);
		}
	}
	return true;
}

void LoginScene::onTouchMoved(Touch * touch, Event * event)
{
}

void LoginScene::onTouchEnded(Touch * touch, Event * event)
{
}

//��ʱ���������״̬ ���ӳɹ�֮�������������
void LoginScene::checkStatus(float time)
{
	if (GameManager::getInstance()->isConnected)
	{
		unschedule(schedule_selector(LoginScene::checkStatus));
		isConnecting = false;
		checkCount = 0;
		Director::getInstance()->replaceScene(GameRoomScene::createScene());
		return;
	}
	checkCount++;
	if (checkCount * SCHEDULE_INTERVAL > CONNECT_TIME_OUT)
	{
		MessageBox("Connect Failed!", "Error");
		unschedule(schedule_selector(LoginScene::checkStatus));
		isConnecting = false;
		checkCount = 0;
		GameManager::getInstance()->player = "";
	}
}