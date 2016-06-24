#pragma once
#include "PokerCard.h"
#ifndef __COCOS_2D_H__
#define __COCOS_2D_H__
#include "cocos2d.h"
#include <thread>
USING_NS_CC;
#endif
#define SERVER_HOST "202.197.66.34"
#define SERVER_PORT 7777
#define BUFFER_SIZE 2048


class GameRoom
{
public:
	GameRoom() {}
	GameRoom(std::string name, int count) { this->name = name; this->count = count; selected = false; };
	std::string name;
	int count;
	bool selected;
};

enum MessageType
{
	//ILLEGAL = 0,
	GET_ROOM_LIST = 1,      //获取房间列表
	GET_ROOM_LIST_RESULT,
	CREATE_ROOM,            //创建房间同时创建用户
	CREATE_ROOM_RESULT,
	ENTER_ROOM,             //进入房间同时创建用户
	ENTER_ROOM_RESULT,
	ENTER_ROOM_OTHERS,      //别人进入房间
	READY,                  //准备游戏
	READY_RESULT,
	READY_OTHERS,           //别人准备
	OUT_CARD,               //自己出牌
	OUT_CARD_RESULT,
	OUT_CARD_OTHERS,        //别人出牌
	START_GAME,             //开始游戏只能够从服务器发出
	END_GAME,               //游戏结束

};


class GameManager 
{
public:
	static GameManager* getInstance();
	
	void initCard(Ref* ref);
	void initNumbers(Ref* ref);
	void increaseCount(Ref* ref);
	void loadResource();
	void threadFunction();
	void release();
	void handleMessage();
	void handleGetRoomListResult();
	void handleCreateRoomResult();
	void handleEnterRoomResult();
	void handleEnterRoomOthers();
	void handleReadyResult();
	void handleReadyOthers();
	void handleOutCardResult();
	void handleOutCardOthers();
	void handleStartGame();
	void handleEndGame();

	static GameManager* instance;
	int numberOfTotalRes;
	int numberOfLoadRes;
	int readBufLength;
	int remainLength;
	bool isConnected;
	char readBuf[BUFFER_SIZE];
	SOCKET sock;

	std::thread socketThread;

	Vector<PokerCard *> rawCardArray;
	Vector<Sprite *> numberArray;
	std::vector<std::string> split(std::string& str, char ch);
	std::vector<GameRoom> roomList;
	/** store all the judge function by their priority **/
private:
	GameManager();
	
}; 