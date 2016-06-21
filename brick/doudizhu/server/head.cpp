#include "head.h"
#include <string.h>

map<string, GameRoom *> roomMap;
map<string, Player*> playerMap;
map<int, Player*> socketMap;
GameRoom::GameRoom()
{
	roomName = "";
	currentPlayers = 0;
	isRunning = 0;
	memset(players, 0, sizeof(Player *) * 3);
}
GameRoom::GameRoom(string name)
{
	roomName = name;
	currentPlayers = 0;
	isRunning = 0;
	memset(players, 0, sizeof(Player *) * 3);
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
	player->identity= UNDEFINED;
	player->ready = 0;
	player->readBufLen = 0;
	player->fd = 0;
	player->remain_length = 0;
	playerMap[name] = player;
	memset(player->readBuf, 0, sizeof(player->readBuf));
	return player;
}
/** 加入房间 **/
GameRoom *Player::joinGame(string roomName)
{
	if(roomMap.find(roomName) == roomMap.end())
	{
		printf("没有房间%s!\n", roomName.data());
		return NULL;
	}

	GameRoom *room = roomMap[roomName];
	if(room->currentPlayers >= 3)
	{
		printf("房间%s已经满员!\n", roomName.data());
		return NULL;
	}
	for(int i = 0; i < 3; i++)
	{
		if(room->players[i] == NULL)
		{
			room->players[i] = this;
			room->currentPlayers = room->currentPlayers + 1;
			return room;
		}
	}
	return NULL;
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
