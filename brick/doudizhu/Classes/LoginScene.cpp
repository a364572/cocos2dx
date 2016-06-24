#include "LoginScene.h"

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

//	auto deleg = CurTextField::create();
	auto input = TextFieldTTF::textFieldWithPlaceHolder("Input name", "arial", 25);
//	input->setDelegate(deleg);
	input->attachWithIME();
	input->setPosition(Point(origin.x + visible.width / 2, origin.y + visible.height * 3 / 4));

	auto listen = EventListenerTouchOneByOne::create();
	listen->onTouchBegan = [](Touch *t, Event *event) {};

	addChild(input);
	return true;
}

Scene * LoginScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoginScene::create();
	scene->addChild(layer);
	return scene;
}

CurTextField * CurTextField::create()
{
	auto field = new CurTextField();
	field->autorelease();
	return field;
}

bool CurTextField::onTextFieldAttachWithIME(TextFieldTTF * pSender)
{
	log("Attach IME");
	return false;
}

bool CurTextField::onTextFieldDetachWithIME(TextFieldTTF * pSender)
{
	log("Detach IME");
	return false;
}

bool CurTextField::onTextFieldInsertText(TextFieldTTF * pSender, const char * text, size_t len)
{
	log("insert text %s", text);
	return false;
}

bool CurTextField::onTextFieldDeleteBackward(TextFieldTTF * pSender, const char * text, size_t len)
{
	log("delete text %s", text);
	return false;
}

void CurTextField::keyboardWillShow(IMEKeyboardNotificationInfo & info)
{
	log("keyBorard will show");
}

void CurTextField::keyboardWillHide(IMEKeyboardNotificationInfo & info)
{
	log("keyBorard will hide");
}
