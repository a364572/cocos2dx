#include "Player.h"

Player* Player::create()
{
	auto player = new Player();
	player->initWithFile("logo_unknown.png");	//Ĭ�ϳ�ʼ����ͷ��

	player->autorelease();
	return player;
}