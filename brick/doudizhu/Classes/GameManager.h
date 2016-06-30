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
#define CONNECT_TIME_OUT	10000
#define SCHEDULE_INTERVAL	1000

/************************************************************************/
/* 基本的逻辑是：
	1. 用户打开程序，显示登录界面，后台连接服务器
	2. 用户输入用户名，点击登入，切换到游戏主界面
	3. 用户选择房间进入，或者用户自己创建新房间
	4. 进入甲板界面，准备开始游戏
	5. 游戏结束，返回房间列表界面*/
/************************************************************************/
class GameRoom
{
public:
	GameRoom() {}
	GameRoom(std::string name, int count, bool inGame) 
	{ 
		this->name = name; 
		this->count = count; 
		this->selected = false;
		this->inGame = inGame;
	};
	std::string name;
	int count;
	bool selected;
	bool inGame;
};

struct SimplePlayer
{
	std::string name;
	int serverPosition;
	bool ready;
};
enum MessageType
{
	//ILLEGAL = 0,
	GET_ROOM_LIST = 1,      //获取房间列表
	GET_ROOM_LIST_RESULT,
	CREATE_PLAYER,			//创建玩家
	CREATE_PLAYER_RESULT,
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

	void connectWithServer();
	void threadFunction();
	void release();

	void sendMessage(std::string message, MessageType type);
	void handleMessage();

	void createPlayer(std::string player);
	void getRoomList();
	void createRoom(std::string room);
	void enterRoom(std::string room);
	void readyGame();
	void outCard(std::vector<PokerCard *> cards);

	void handleCreatePlayerResult();
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
	std::vector<std::string> splitString(std::string& str, char ch);

	static GameManager* instance;
	int numberOfTotalRes;
	int numberOfLoadRes;
	int readBufLength;
	int remainLength;
	int serverPosition;
	int isWaitingRoom;		//0表示没在等 1表示在等 2表示成功 3表示失败
	int isWaitingList;
	int isWaitingReady;
	int isInGame;			//0表示没开始 1表示准备好开始了 2表示正在游戏中
	int isWaitingPlayer; 
	bool isConnected;
	char readBuf[BUFFER_SIZE];
	SOCKET sock;

	std::thread socketThread;

	Vector<PokerCard *> rawCardArray;
	Vector<Sprite *> numberArray;
	std::vector<GameRoom> roomList;
	std::vector<std::string> readyPlayerList;
	std::vector<SimplePlayer> enterPlayerList;
	std::string player;
	std::string roomName;
private:
	GameManager();
	
}; 