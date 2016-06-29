#pragma once
#include "cocos2d.h"
USING_NS_CC;

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

