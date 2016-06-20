#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <map>

using namespace std;

class GameRoom;
class Player;
map<string, GameRoom *> roomMap;
map<string, Player*> playerMap;
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
	ENTER_ROOM,		//进入房间同时创建用户
	ENTER_ROOM_RESULT,
	READY,			//准备游戏
	READY_RESULT,
	OUT_CARD,		//自己出牌
	OUT_CARD_RESULT,
	OUT_CARD_OTHERS,	//别人出牌
	START_GAME,		//开始游戏只能够从服务器发出

};
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
	int	flag;
	string 	playerName;
	string	roomName;
	int joinGame(string roomName);
	int exitGame();
};

class GameRoom
{
public:
	string 	roomName;
	Player*	players[3];
	int 	currentPlayers;
	bool	isRunning;
	GameRoom();
	GameRoom(string name, Player *player)
}


GameRoom* addGameRoom(string name, Player* player);
Player* addPlayer(string name);
void deletePlayer(Player* player);
