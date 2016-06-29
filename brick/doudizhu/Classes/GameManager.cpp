#include "GameManager.h"
#include "DeckScene.h"

GameManager* GameManager::instance = nullptr;
GameManager* GameManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new GameManager();
		PokerArrayUtil::initFunction();
	}
	return instance;
}

void GameManager::increaseCount(Ref *ref)
{
	numberOfLoadRes++;
}
void GameManager::loadResource()
{
	Director::getInstance()->getTextureCache()->addImageAsync("logo.png", CC_CALLBACK_1(GameManager::increaseCount, this));
	Director::getInstance()->getTextureCache()->addImageAsync("logo_dizhu.png", CC_CALLBACK_1(GameManager::increaseCount, this));
	Director::getInstance()->getTextureCache()->addImageAsync("logo_nongmin.png", CC_CALLBACK_1(GameManager::increaseCount, this));
	Director::getInstance()->getTextureCache()->addImageAsync("logo_unknown.png", CC_CALLBACK_1(GameManager::increaseCount, this));
	Director::getInstance()->getTextureCache()->addImageAsync("poke_back_header.png", CC_CALLBACK_1(GameManager::increaseCount, this));
	Director::getInstance()->getTextureCache()->addImageAsync("poker_number.png", CC_CALLBACK_1(GameManager::initNumbers, this));
	Director::getInstance()->getTextureCache()->addImageAsync("puke_whole.png", CC_CALLBACK_1(GameManager::initCard, this));
}
void GameManager::connectWithServer()
{
	if (isConnected)
	{
		return;
	}
	//创建后台线程和服务器进行通信
	instance->socketThread = std::thread(&GameManager::threadFunction, this);
	instance->socketThread.detach();

}
void GameManager::threadFunction()
{
	log("Thread start!\n");
	//加载WinSock库
	WSADATA	wsaData;
	WORD version = MAKEWORD(1, 1);
	int err = WSAStartup(version, &wsaData);
	if (err != 0)
	{
		log("Failed to initialize Win Socket!");
		return;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		log("The version of protocol is error!");
		WSACleanup();
		return;
	}

	//创建socket并连接服务器
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock <= 0)
	{
		log("Failed to create socket!");
		return;
	}
	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.S_un.S_addr = inet_addr(SERVER_HOST);
	sockaddr.sin_port = htons(SERVER_PORT);
	err = connect(sock, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr));
	if (err != 0)
	{
		log("Failed to connect with %s:%d", SERVER_HOST, SERVER_PORT);
		return;
	}
	log("Connected with %s:%d", SERVER_HOST, SERVER_PORT);
	remainLength = 0;
	readBufLength = 0;

	//首先向服务器发送创建玩家信息,创建成功之后才将连接置为true
	log("Player:%s", player.data());
	createPlayer(player);

	log("Ready to receive message!");
	char buf[1];
	while (isConnected || isWaitingPlayer)
	{
		//重新接收消息
		if (remainLength == 0) {
			//接收一个字节
			err = recv(sock, buf, 1, 0);
			if (err < 0)
			{
				log("Disconnected!");
				release();
				break;
			}
			if (err == 0)
			{
				continue;
			}
			int msgLen = buf[0] - 1;
			remainLength = msgLen;
			readBufLength = 0;
			log("receive new message length %d", msgLen + 1);
		}
		//继续接收消息
		else
		{
			err = recv(sock, readBuf + readBufLength, remainLength, 0);
			if (err < 0)
			{
				release();
				break;
			}
			if (err == 0)
			{
				continue;
			}
			readBufLength += err;
			remainLength -= err;
			log("receive %d more message", err);
		}
		if (remainLength == 0)
		{
			handleMessage();
		}
	}

}
void GameManager::sendMessage(std::string message, MessageType type)
{
	//消息长度包括类型
	char ch = type;
	message = ch + message;
	ch = message.size();
	if (ch > 254)
	{
		MessageBox("消息内容过长", "错误");
		return;
	}

	message = ch + message;
	if (isConnected || isWaitingPlayer)
	{
		int len = 0;
		int res = 0;
		while (len < message.size())
		{
			res = send(sock, message.data() + len, message.size() - len, 0);
			if (res < 0)
			{
				MessageBox("发送数据失败", "错误");
				release();
			}
			len += res;
		}
		log("send success. length:%d", message.size());
	}

}
void GameManager::release()
{
	log("Disconnect!");
	closesocket(sock);
	sock = -1;
	isConnected = false;
	isWaitingPlayer = false;
	isWaitingRoom = false;
	remainLength = 0;
	readBufLength = 0;
}
void GameManager::handleMessage()
{
	readBuf[readBufLength] = '\0';
	std::string str = readBuf + 1;
	int msgType = readBuf[0];
	log("Receive new message type %d", msgType);
	switch (msgType) {
	case GET_ROOM_LIST_RESULT:
		handleGetRoomListResult();
		break;
	case CREATE_PLAYER_RESULT:
		handleCreatePlayerResult();
		break;
	case CREATE_ROOM_RESULT:
		handleCreateRoomResult();
		break;
	case ENTER_ROOM_RESULT:
		handleEnterRoomResult();
		break;
	case ENTER_ROOM_OTHERS:
		handleEnterRoomOthers();
		break;
	case READY_RESULT:
		handleReadyResult();
		break;
	case READY_OTHERS:
		handleReadyOthers();
		break;
	case OUT_CARD_RESULT:
		handleOutCardResult();
		break;
	case OUT_CARD_OTHERS:
		handleOutCardOthers();
		break;
	case START_GAME:
		handleStartGame();
		break;
	case END_GAME:
		handleEndGame();
		break;
	}
	remainLength = 0;
	readBufLength = 0;
}

void GameManager::createPlayer(std::string player)
{
	this->isWaitingPlayer = true;
	sendMessage(player, CREATE_PLAYER);
}

void GameManager::getRoomList()
{
	if (this->isWaitingList > 0)
	{
		return;
	}
	this->roomList.clear();
	this->isWaitingList = 1;
	sendMessage("", GET_ROOM_LIST);
}

void GameManager::createRoom(std::string room)
{
	if (this->isWaitingRoom > 0)
	{
		return;
	}
	this->isWaitingRoom = 1;
	sendMessage(room, CREATE_ROOM);
}

void GameManager::enterRoom(std::string room)
{
	if (this->isWaitingRoom > 0)
	{
		return;
	}
	this->isWaitingRoom = 1;
	sendMessage(room, ENTER_ROOM);
}

void GameManager::ready()
{
	sendMessage("", READY);
}

void GameManager::outCard(std::vector<PokerCard *> cards)
{
	std::string msg = "";
	for (auto card : cards)
	{
		char value = card->getValueInAll();
		msg.push_back(value);
		msg.push_back(' ');
	}
	sendMessage(msg, OUT_CARD);
}

void GameManager::handleCreatePlayerResult()
{
	if (isWaitingPlayer)
	{
		isWaitingPlayer = false;
		std::string flag = readBuf + 1;
		if (flag == "1")
		{
			log("create player success");
			isConnected = true;
		}
		else
		{
			log("Failed to create player because length %d %s!", flag.size(), flag.data());
		}
	}
	log("No waiting for player");
}

void GameManager::handleGetRoomListResult()
{
	//将字符串分割 更新房间列表数据
	std::string list(readBuf + 1);
	auto rooms = splitString(list, '\n');
	for (auto str : rooms)
	{
		auto result = splitString(str, ' ');
		if (result.size() >= 3)
		{
			int count;
			bool inGame;
			std::stringstream ss;
			ss << result[1];
			ss >> count;

			ss.clear();
			ss << result[2];
			ss >> inGame;
			GameRoom room(result[0], count, inGame);
			roomList.push_back(room);
		}
	}
	log("Current rooms %d", roomList.size());
	this->isWaitingList = 2;
}
void GameManager::handleCreateRoomResult()
{
	if (isWaitingRoom == 1)
	{
		std::string flag = readBuf + 1;
		if (flag == "1")
		{
			isWaitingRoom = 2;
			roomName = player;
		}
		else
		{
			isWaitingRoom = 3;
		}
	}
}
void GameManager::handleEnterRoomResult()
{
	if (isWaitingRoom == 1)
	{
		std::string flag = readBuf + 1;
		if (flag.size() > 0)
		{
			isWaitingRoom = 2;
			roomName = flag;
		}
		else
		{
			isWaitingRoom = 3;
		}
	}
}
void GameManager::handleEnterRoomOthers()
{
}
void GameManager::handleReadyResult()
{
}
void GameManager::handleReadyOthers()
{
}
void GameManager::handleOutCardResult()
{
}
void GameManager::handleOutCardOthers()
{
}
void GameManager::handleStartGame()
{
}
void GameManager::handleEndGame()
{
}
void GameManager::initCard(Ref * ref)
{
	int type = 0;
	auto texture = Director::getInstance()->getTextureCache()->getTextureForKey("puke_whole.png");
	auto cardWidth = texture->getContentSize().width / 9;
	auto cardHeight = texture->getContentSize().height / 6;
	for (int i = 0; i < 54; i++)
	{
		if (i % 13 == 0)
		{
			type++;
		}
		int row = i / 9;
		int col = i - 9 * row;

		auto originX = texture->getContentSize().width - (col + 1) * cardWidth;
		auto originY = texture->getContentSize().height - (6 - row) * cardHeight;
		Rect rect(originX, originY, cardWidth, cardHeight);
		auto card = PokerCard::create(SpriteFrame::createWithTexture(texture, rect));
		card->setType(type);
		card->setValueInAll(i);
		int value = i % 13 + 1;
		//set the value of card beyond three with its number
		if (value > 2)
		{
			card->setValueInType(value);
		}
		//set the value of card 1 and card 2 as 14 and 15 respectively
		else
		{
			card->setValueInType(value + 13);
		}
		//set the value of small queen and big queen as 16 and 16 respectively
		if (i >= 52)
		{
			card->setValueInType(i % 13 + 16);
		}
		rawCardArray.pushBack(card);
	}
	log("Total Card : %d", rawCardArray.size());
	increaseCount(ref);
}

void GameManager::initNumbers(Ref * ref)
{
	auto texture = Director::getInstance()->getTextureCache()->getTextureForKey("poker_number.png");
	auto width = texture->getContentSize().width / 10;
	auto height = texture->getContentSize().height;
	for (int i = 0; i < 10; i++)
	{
		auto sprite = Sprite::createWithTexture(texture, Rect(i * width, 0, width, height));
		numberArray.pushBack(sprite);
	}
	increaseCount(ref);
}

std::vector<std::string> GameManager::splitString(std::string & str, char ch)
{
	int prev = 0;
	std::vector<std::string> result;
	for(int i = 0; i < str.size(); i++)
	{
		if (str[i] == ch)
		{
			if (i > prev) 
			{
				result.push_back(str.substr(prev, i - prev));
			}
			prev = i + 1;
		}
	}
	result.push_back(str.substr(prev));
	return result;
}

GameManager::GameManager()
{
	//初始化变量
	numberOfTotalRes = 7;
	numberOfLoadRes = 0;
	isConnected = false;
	isWaitingPlayer = false;
	isWaitingRoom = 0;
	isWaitingList = 0;
	readBufLength = 0;
	remainLength = 0;
	srand(time(nullptr));

	//roomList.push_back(GameRoom("room1", 2));
	//roomList.push_back(GameRoom("room2", 3));
	//roomList.push_back(GameRoom("room3", 2));
	//roomList.push_back(GameRoom("room4", 1));
	//roomList.push_back(GameRoom("room5", 1));
	//roomList.push_back(GameRoom("room6", 1));
	//roomList.push_back(GameRoom("room7", 1));
	//roomList.push_back(GameRoom("room8", 1));
	//roomList.push_back(GameRoom("room9", 1));
	//roomList.push_back(GameRoom("room10", 1));
	//roomList.push_back(GameRoom("room11", 1));
	//roomList.push_back(GameRoom("room12", 1));
	//roomList.push_back(GameRoom("room13", 1));
	//roomList.push_back(GameRoom("room14", 1));
	//roomList.push_back(GameRoom("room15", 1));
	//roomList.push_back(GameRoom("room16", 1));
	//roomList.push_back(GameRoom("room17", 1));
	//roomList.push_back(GameRoom("room18", 1));
	//roomList.push_back(GameRoom("room19", 1));
	//roomList.push_back(GameRoom("room20", 1));
}
