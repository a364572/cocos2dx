#pragma once
#include "cocos2d.h"
#include "PokerCard.h"
USING_NS_CC;
class Player : public Sprite 
{
public:
	static Player* create();
protected:
private:
	Vector<PokerCard *> leftCards;		//ʣ�����
	Vector<PokerCard *> outCards;		//�ѳ�����
	int position;						//�ڷ����е�λ��
	std::string name;					//�������
};
