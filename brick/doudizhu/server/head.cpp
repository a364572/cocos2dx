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
	memset(players, 0, sizeof(Player *) * ROOM_SIZE);
	for(int i = 1; i <= 54; i++)
	{
		randCards.push_back(i);
	}
}
GameRoom::GameRoom(string name)
{
	roomName = name;
	currentPlayers = 0;
	isRunning = 0;
	memset(players, 0, sizeof(Player *) * ROOM_SIZE);
	for(int i = 1; i <= 54; i++)
	{
		randCards.push_back(i);
	}
}

Player::Player()
{
	playerName = "";
	room = NULL;
	identity = UNDEFINED;
	readBufLen = 0;
	remain_length = 0;
	fd = 0;
	position = -1;
	ready = false;
	called = robbed = false;
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
	if(room->currentPlayers >= ROOM_SIZE)
	{
		printf("房间%s已经满员!\n", roomName.data());
		return NULL;
	}
	for(int i = 0; i < ROOM_SIZE; i++)
	{
		if(room->players[i] == NULL)
		{
			printf("玩家%s加入房间%s\n", playerName.data(), roomName.data());
			this->room = room;
			this->position = i;
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
	if(!this->room)
	{
		return NONE;
	}
	GameRoom *room = this->room;
	room->isRunning = false;

	//没人的话删除房间
	room->currentPlayers--;
	if(room->currentPlayers <= 0)
	{
		printf("删除房间%s\n", room->roomName.data());
		roomMap.erase(room->roomName);
		delete room;
	}
	this->room = NULL;
	this->position = -1;
	this->ready = false;
}
