#include "Player.h"

Player* Player::create()
{
	auto player = new Player();
	player->initWithFile("logo_unknown.png");	//默认初始化的头像

	player->autorelease();
	return player;
}