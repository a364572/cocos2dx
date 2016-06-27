#pragma once
#include "cocos2d.h"
USING_NS_CC;

#define CONNECT_TIME_OUT	10000
#define SCHEDULE_INTERVAL	1000
class CurTextField : public TextFieldTTF, public TextFieldDelegate
{
public:
	static CurTextField *create();

	//delegate 方法
	virtual bool onTextFieldAttachWithIME(TextFieldTTF* pSender);
	virtual bool onTextFieldDetachWithIME(TextFieldTTF* pSender);
	virtual bool onTextFieldInsertText(TextFieldTTF* pSender, const char* text, size_t len);
	virtual bool onTextFieldDeleteBackward(TextFieldTTF* pSender, const char* text, size_t len);

	//IME方法
	virtual void keyboardWillShow(IMEKeyboardNotificationInfo& info);
	virtual void keyboardWillHide(IMEKeyboardNotificationInfo& info);


};
class LoginScene : public Layer
{
public:
	virtual bool init();
	static Scene* createScene();
	CREATE_FUNC(LoginScene);

	virtual bool onTouchBegan(Touch *touch, Event *event);
	virtual void onTouchMoved(Touch *touch, Event *event);
	virtual void onTouchEnded(Touch *touch, Event *event);

	void checkStatus(float time);

	TextFieldTTF *inputText;
	Sprite* loginBtn;
	int checkCount;
	bool isConnecting;
};

