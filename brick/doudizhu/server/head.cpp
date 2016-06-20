#include "head.h"

extern map<string, GameRoom*> roomMap;
extern map<string, Player*> playerMap;
GameRoom::GameRoom()
{
	roomName = "";
	currentPlayers = 0;
	isRunning = false;
}
GameRoom::GameRoom(string name, Player* player)
{
	this->roomName = name;
	this->players[0] = player;
	currentPlayers = 1;
	isRunning = false;
}

/** 创建房间并放入map中 **/
GameRoom* addGameRoom(string name, Player* player)
{
	if(roomMap.find(name) == roomMap.end())
	{
		GameRoom* room = new GameRoom(name, player);
		roomMap[name] = room;
		return room;
	}
	return NULL;
}

/** 创建玩家 **/
Player* addPlayer(string name)
{
	if(playerMap.find(name) != playerMap.end())
	{
		return NULL;
	}
	Player* player = new Player();
	player->playerName = name;
	player->flag = UNDEFINED;
	playerMap[name] = player;
	return player;
}
/** 加入房间 **/
int Player::joinGame(string roomName)
{
	if(roomMap.find(roomName) == roomMap.end())
	{
		return NONE;
	}

	GameRoom *room = roomMap[roomName];
	if(room->currentPlayers >= 3)
	{
		return FULL;
	}
	room->players[room->currentPlayers] = this;
	room->currentPlayers = room->currentPlayers + 1;
	return LEGAL;
}

/** 退出房间 **/
int Player::exitGame()
{
	if(this->roomName == "")
	{
		return NONE;
	}
	GameRoom *room = roomMap[this->roomName];
	room->isRunning = false;

	//没人的话删除房间
	room->currentPlayers--;
	if(room->currentPlayers == 0)
	{
		roomMap.erase(this->roomName);
		delete room;
	}
}
