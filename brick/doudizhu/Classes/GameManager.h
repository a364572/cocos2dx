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
/* �������߼��ǣ�
	1. �û��򿪳�����ʾ��¼���棬��̨���ӷ�����
	2. �û������û�����������룬�л�����Ϸ������
	3. �û�ѡ�񷿼���룬�����û��Լ������·���
	4. ����װ���棬׼����ʼ��Ϸ
	5. ��Ϸ���������ط����б����*/
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
	GET_ROOM_LIST = 1,      //��ȡ�����б�
	GET_ROOM_LIST_RESULT,
	CREATE_PLAYER,			//�������
	CREATE_PLAYER_RESULT,
	CREATE_ROOM,            //��������ͬʱ�����û�
	CREATE_ROOM_RESULT,
	ENTER_ROOM,             //���뷿��ͬʱ�����û�
	ENTER_ROOM_RESULT,
	ENTER_ROOM_OTHERS,      //���˽��뷿��
	READY,                  //׼����Ϸ
	READY_RESULT,
	READY_OTHERS,           //����׼��
	OUT_CARD,               //�Լ�����
	OUT_CARD_RESULT,
	OUT_CARD_OTHERS,        //���˳���
	START_GAME,             //��ʼ��Ϸֻ�ܹ��ӷ���������
	END_GAME,               //��Ϸ����

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
	int isWaitingRoom;		//0��ʾû�ڵ� 1��ʾ�ڵ� 2��ʾ�ɹ� 3��ʾʧ��
	int isWaitingList;
	int isWaitingReady;
	int isInGame;			//0��ʾû��ʼ 1��ʾ׼���ÿ�ʼ�� 2��ʾ������Ϸ��
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