#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

#define SERVER_HOST "0.0.0.0"
#define SERVER_PORT 7777
#define BUFFER_SIZE 2048
#define ROOM_SIZE 3
#define MAX_CALL_COUNT 4
class GameRoom;
class Player;
extern map<string, GameRoom *> roomMap;
extern map<string, Player*> playerMap;
extern map<int, Player*> socketMap;
enum RoomFlag
{
	LEGAL = 0,
	NONE,
	EMPTY,
	FULL,
};
enum PokerType
{
	FANGKUAI = 1,
	MEIHUA,
	HONGXIN,
	HEITAO,
	DAXIAOWANG
};
enum PlayerType
{
	UNDEFINED,
	DIZHU = 1,
	NONGMING,
};
enum MessageType
{
	ILLEGAL = 0,
	GET_ROOM_LIST = 1,	//获取房间列表
	GET_ROOM_LIST_RESULT,
	CREATE_PLAYER,		//创建玩家
	CREATE_PLAYER_RESULT,
	CREATE_ROOM,		//创建房间
	CREATE_ROOM_RESULT,
	ENTER_ROOM,		//进入房间
	ENTER_ROOM_RESULT,
	ENTER_ROOM_OTHERS,	//别人进入房间
	READY,			//准备游戏
	READY_RESULT,
	READY_OTHERS,		//别人准备
	CALL_DIZHU,		//叫地主
	CALL_DIZHU_RESULT,
	CALL_DIZHU_OTHERS,	//别人叫地主
	ROB_DIZHU,		//抢地主
	ROB_DIZHU_RESULT,
	ROB_DIZHU_OTHERS,	//别人抢地主
	OUT_CARD,		//自己出牌
	OUT_CARD_RESULT,
	OUT_CARD_OTHERS,	//别人出牌
	START_OUT_CARD,		//叫完地主开始出牌
	START_GAME,		//开始游戏只能够从服务器发出
	END_GAME,		//游戏结束

};

/** 客户端发送的消息格式
 * 长度	1字节
 * 类型	1字节
 * 	获取房间列表：
 * 		NONE
 * 	创建玩家
 * 		玩家名字
 * 	创建房间
 * 		房间名称
 * 	进入房间：
 *		房间名称
 * 	准备游戏：
 *		NONE
 *	叫地主：
 *		叫OR不叫
 *	抢地主：
 *		抢OR不抢
 * 	出牌：
 * 		牌列表，每张牌占用一个字节
 *
 *
 **/

/** 服务器发送的消息格式
 * 长度	1字节
 * 类型	1字节
 * 	获取房间列表：
 * 		房间名称 空格分割
 * 	创建玩家
 * 		失败OR成功
 * 	创建房间：
 * 		失败OR成功
 * 	进入房间：
 * 		其它玩家的名称 空格 准备状态 换行符
 * 		...
 * 	别人进入房间：
 *		玩家名称 空格 玩家位置 空格 准备状态
 * 	准备游戏：
 * 		NONE
 * 	别人准备游戏：
 * 		玩家名称
 * 	叫地主：
 * 		失败OR成功
 * 	别人叫地主：
 * 		玩家名称
 * 	出牌：
 * 		成功或是失败 空格 牌列表：每张占用一个字节
 * 	别人出牌：
 * 		玩家名称 空格 牌列表
 * 	开始出牌：
 * 		玩家名称
 * 	开始游戏：
 * 		牌列表
 * 	游戏结束：
 * 		玩家 是否是赢家 剩余的牌列表 换行符
 *		...
 **/

class PokerCard
{
public:
	bool isQueen()
	{
		if(this->valueInAll >= 52)
		{
			return true;
		}
		return false;
	}
	int	type;
	int	valueInType;
	int	valueInAll;
};
class Player
{
public:
	int		identity;		//身份
	int		readBufLen;
	int		fd;			//对应的socket描述符
	int 		remain_length;
	int 		position;
	bool		ready;			//是否准备
	bool		called;			//是否叫过地主
	bool		robbed;			//是否抢过地主
	char		readBuf[BUFFER_SIZE];	//每个玩家都有一个消息缓冲区
	string 		playerName;
	GameRoom* 	room;

	Player();
	GameRoom* 	joinGame(string roomName);
	int 		exitGame();
};

class GameRoom
{
public:
	string 		roomName;
	Player*		players[3];
	int 		currentPlayers;
	int		isRunning;
	int 		currentCallIndex;	//哪个人叫地主
	int		currentRobIndex;	//哪个人抢地主
	int		currentRobCount;	//目前抢了多少次地主
	int		currentOutIndex;	//哪个人出牌
	int		currentOutType;		//这一轮的出牌类型
	int		currentRoundCount;	//本轮的出牌次数
	int		indexOfDizhu;		//地主的下标
	vector<int> 	randCards;
	GameRoom();
	GameRoom(string name);
};

Player* addPlayer(string name);
void deletePlayer(Player* player);
void sendRoomList(int fd);
void handle_message(int fd);			//处理用户消息
void handle_get_room_list(Player *player);	//获取房间列表处理函数
void handle_create_player(Player* player);	//玩家注册处理函数
void handle_create_room(Player* player);	//创建房间处理函数
void handle_enter_room(Player *player);		//进入房间处理函数
void handle_ready(Player *player);		//玩家准备处理函数
void handle_call_dizhu(Player* player);		//玩家叫地主
void handle_rob_dizhu(Player* player);	//玩家抢地主
void handle_out_card(Player *player);		//玩家出牌处理函数
