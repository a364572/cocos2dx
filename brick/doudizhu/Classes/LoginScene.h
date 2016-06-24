#pragma once
#include "cocos2d.h"
USING_NS_CC;

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
};

