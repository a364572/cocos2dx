#include "head.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sstream>
#include <pthread.h>
#include <time.h>
#include <algorithm>

void handle_accept(struct epoll_event, int epoll_fd);
void handle_others(struct epoll_event, int epoll_fd);
void sendMessage(int fd, int type, string message);
void releasePlayer(int fd);
void* handleInput(void *);

int main()
{
	struct sockaddr_in server;
	int listenfd;
	int epoll_fd;
	bool running = true;
	pthread_t pid;

	//设置监听套接字
	listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(listenfd <= 0)
	{
		perror("Failed to create socket!");
		return -1;
	}
	memset((char *)&server, 0 , sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(SERVER_HOST);
	server.sin_port = htons(SERVER_PORT);

	if(bind(listenfd, (struct sockaddr*)&server, sizeof(struct sockaddr)) != 0)
	{
		printf("Failed to bind in %s:%d", SERVER_HOST, SERVER_PORT);
		return -1;
	}

	listen(listenfd, 10);

	roomMap["test"] = new GameRoom("test");
	Player *play = new Player();
	play->fd = -1;
	play->playerName = "testUser";
	play->joinGame("test");
	socketMap[-1] = play;
	playerMap["testUser"] = play;

	//初始化epoll
	srand(time(NULL));
	epoll_fd = epoll_create(100);
	if(epoll_fd <= 0)
	{
		perror("Falied to create epoll fd!");
		close(listenfd);
		return -1;
	}
	struct epoll_event ev;
	ev.data.fd = listenfd;
	ev.events = EPOLLIN;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenfd, &ev);

	//创建后台进程监听用户输入
	pthread_create(&pid, NULL, handleInput, NULL);
	//循环监听事件
	struct epoll_event events[100];
	while(running)
	{
		int numbers = epoll_wait(epoll_fd, events, 100, -1);
		int i = 0;
		for(; i < numbers; i++)
		{
			if(events[i].data.fd == listenfd)
			{
				//新的连接
				handle_accept(events[i], epoll_fd);
			}
			else
			{
				//接收数据
				handle_others(events[i], epoll_fd);
			}
		}
	}
}

void* handleInput(void *arg)
{
	char cmd[128];
	pthread_detach(pthread_self());
	while(1)
	{
		memset(cmd, 0, sizeof(cmd));
		scanf("%s", cmd);
		if(0 == strcmp(cmd, "room"))
		{
			printf("当前的房间列表如下：\n");
			printf("%10s|%10s|%10s\n", "Name", "Count", "Running");
			map<string, GameRoom*>::iterator ite = roomMap.begin();
			while(ite != roomMap.end())
			{
				printf("%10s|%10d|%10d\n", ite->second->roomName.data(),
					ite->second->currentPlayers,
					ite->second->isRunning);
				ite++;
			}
		}
		else if(0 == strcmp(cmd, "player"))
		{
			printf("当前的玩家列表如下：\n");
			printf("%15s|%10s|%10s|%10s\n", "Description", "Name", "Room", "Identity");
			map<int, Player*>::iterator ite = socketMap.begin();
			while(ite != socketMap.end())
			{
				Player* player = ite->second;
				printf("%15d|%10s|%10s|%10d\n", player->fd,
					player->playerName.data(), player->room->roomName.data(),
					player->identity);
				ite++;
			}
		}
		else if(0 == strcmp(cmd, "clear"))
		{
			system("reset");
		}
	}
}
void sendMessage(int fd, int type, string message)
{
	int length = message.size() + 2;
	if(length >= 255)
	{
		printf("发给%d的消息过长:%s\n", fd, message.c_str());
		return;
	}
	message = (char)type + message;
	message = (char)length + message;

	printf("准备发送类型为%d的消息给%d,长度%ld\n", type, fd, message.size());
//	printf("消息内容:");
//	for(unsigned long i = 0; i < message.size(); i++)
//	{
//		printf("%d ", (int)message.at(i));
//	}
//	printf("\n");
	length = 0;
	int res = 0;
	while(length < (int)message.size())
	{
		res = send(fd, message.data() + length, message.size() - length, 0);
		if(res < 0)
		{
			perror("");
			//断开连接 删除用户和对应的资源
			Player* player = socketMap[fd];
			if(!player)
			{
				printf("描述符%d不合法\n", fd);
			}
			else
			{
				printf("玩家：%d名称：%s断开连接\n", fd, player->playerName.data());
				releasePlayer(fd);
			}
			return;
		}
		length += res;
	}
//	printf("消息长度%d已经发送\n", (int)message.size());
}

void sendRoomList(int fd)
{
	string message = "";
	map<string, GameRoom *>::iterator ite = roomMap.begin();
	while(ite != roomMap.end())
	{
		stringstream ss;
		string str;
		message.append(ite->first);
		message.append(" ");

		ss << ite->second->currentPlayers;
		ss >> str;
		message.append(str);
		message.append(" ");

		ss.clear();
		ss << ite->second->isRunning;
		ss >> str;
		message.append(str);
		message.append("\n");
		ite++;
	}
	sendMessage(fd, GET_ROOM_LIST_RESULT, message);
}

void handle_message(int fd)
{
	if(socketMap.find(fd) == socketMap.end())
	{
		printf("没有对应的玩家%d\n", fd);
		return;
	}

	Player *player = socketMap[fd];
	int messageType = player->readBuf[0];
	printf("收到来自玩家%s类型为%d的消息\n", player->playerName.data(), messageType);
	switch(messageType){
	case GET_ROOM_LIST:
		handle_get_room_list(player);
		break;
	case ENTER_ROOM:
		handle_enter_room(player);
		break;
	case CREATE_PLAYER:
		handle_create_player(player);
		break;
	case CREATE_ROOM:
		handle_create_room(player);
		break;
	case READY:
		handle_ready(player);
		break;
	case CALL_DIZHU:
		handle_call_dizhu(player);
		break;
	case ROB_DIZHU:
		handle_rob_dizhu(player);
		break;
	case OUT_CARD:
		//handle_out_card(player);
		break;
	}
	//清空玩家的缓冲区
	player->readBufLen = 0;

}

void handle_get_room_list(Player* player)
{
	sendRoomList(player->fd);
}

/**
 * 成功返回1 失败返回0
 **/
void handle_create_player(Player* player)
{
	string name = player->readBuf + 1;
	//如果存在相同名字的玩家
	if(playerMap.find(name) != playerMap.end())
	{
		printf("玩家名字与%d重复\n", playerMap[name]->fd);
		sendMessage(player->fd, CREATE_PLAYER_RESULT,
				"0");
	}
	else
	{
		printf("玩家%s创建成功\n", name.data());
		playerMap[name] = player;
		player->playerName = name;
		sendMessage(player->fd, CREATE_PLAYER_RESULT,
				"1");
	}

//	printf("当前的房间有%ld个", roomMap.size());
//	printf("当前的用户有%ld个:", socketMap.size());
//	map<int, Player*>::iterator ite = socketMap.begin();
//	while(ite != socketMap.end())
//	{
//		string name = ite->second->playerName;
//		printf("%s ", name.data());
//		ite++;
//	}
//	printf("\n");
}
void handle_create_room(Player* player)
{
	//缓冲区的第一个字节是消息类型
	string name = player->readBuf + 1;
	if(roomMap.find(name) != roomMap.end())
	{
		printf("创建房间失败，房间%s已经存在\n", name.data());
		sendMessage(player->fd, CREATE_ROOM_RESULT, "0");
		return;
	}
	GameRoom *room = new GameRoom(name);
	roomMap[name] = room;
	room = player->joinGame(name);

	//成功的话返回：房间名称 换行 在房间中的位置
	//否则返回：-1
	string message;
	if(room)
	{
		stringstream ss;
		ss << player->position;
		ss >> message;
		message = name + "\n" + message;
		printf("用户%s创建房间%s成功\n", player->playerName.data(), name.data());
	}
	else
	{
		message = "-1";
		printf("用户%s创建房间%s失败\n", player->playerName.data(), name.data());
		player->exitGame();
	}
	sendMessage(player->fd, CREATE_ROOM_RESULT, message);

}
void handle_enter_room(Player* player)
{
	//缓冲区的第一个字节是消息类型
	string name = player->readBuf + 1;
	GameRoom *room = player->joinGame(name);
	if(room)
	{
		//要发送两类信息 一个是给当前用户 另一个提醒其他用户有人进入
		//message2 只有一行 玩家名称 空格 位置 空格 是否准备
		//message1 第一行返回房间名称 第二行返回在房间中的位置 然后每一行一个用户
		string message1 = player->room->roomName + "\n";
		stringstream ss;
		string str;
		ss << player->position;
		ss >> str;
		message1.append(str + "\n");

		string message2 = player->playerName + " ";
		str.clear();
		ss.clear();
		ss << player->position;
		ss >> str;
		message2.append(" " + str + " 0\n");
		for(int i = 0; i < ROOM_SIZE; i++)
		{
			Player* tmp = room->players[i];
			if(tmp)
			{
				if(tmp->fd != player->fd)
				{
					//先给每个小伙伴发送一条进入房间的消息
					sendMessage(tmp->fd, ENTER_ROOM_OTHERS, message2);
				}
				//然后增加其它玩家名称 位置 是否准备
				message1.append(tmp->playerName + " ");
				ss.clear();
				str.clear();
				ss << tmp->position;
				ss >> str;
				message1.append(str + " ");
				if(tmp->ready)
				{
					message1.append("1\n");
				}
				else
				{
					message1.append("0\n");
				}
			}
		}
		printf("玩家%s加入房间%s成功\n",
			player->playerName.data(), name.data());
		sendMessage(player->fd, ENTER_ROOM_RESULT, message1);
	}
	else
	{
		printf("玩家%s加入房间%s失败\n",
			player->playerName.data(), name.data());
		sendMessage(player->fd, ENTER_ROOM_RESULT, "");
	}
}

void handle_ready(Player* player)
{
	if(!player->room)
	{
		player->exitGame();
		printf("用户%s不在房间%s\n", player->playerName.data(),
			player->room->roomName.data());
		sendMessage(player->fd, READY_RESULT, "0");
		return;
	}

	player->ready = true;
	sendMessage(player->fd, READY_RESULT, "1");
	GameRoom* room = player->room;
	int readyCount = 0;
	for(int i = 0; i < ROOM_SIZE; i++)
	{
		Player* play = room->players[i];
		if(play && play->fd != player->fd)
		{
			sendMessage(play->fd, READY_OTHERS, player->playerName);
		}
		if(play && play->ready)
		{
			readyCount++;
		}
	}
	//如果所有人都准备了 开始游戏
	if(readyCount == ROOM_SIZE)
	{
		//因为牌的下标有0 会截断 所以牌是从1开始
		string cards = "";
		random_shuffle(room->randCards.begin(), room->randCards.end());
		for(int i = 0; i < 54; i++)
		{
			cards.push_back((char)room->randCards.at(i));
		}
		//给每个玩家发送开始游戏消息
		for(int i = 0; i < ROOM_SIZE; i++)
		{
			Player* play = room->players[i];
			if(play)
			{
				sendMessage(play->fd, START_GAME, cards);			}
		}
		room->currentPlayers = 0;
		room->currentOutType = 0;
		room->currentOutIndex = 0;
		room->currentRobCount = 0;
		room->currentRobIndex = 0;
		room->currentCallIndex = 0;
		room->currentRoundCount = 0;
	}
}

/**
 * 每次接收一个socket，创建一个临时Player
 **/
void handle_accept(struct epoll_event ev, int epoll_fd)
{
	int listen_fd = ev.data.fd;
	if(ev.events == EPOLLIN)
	{
		//接收并发送房间列表
		struct sockaddr_in client;
		socklen_t len = 0;
		memset((char *)&client, 0, sizeof(client));
		int fd = accept(listen_fd, (struct sockaddr*)&client, &len);
		if(fd > 0)
		{
			printf("新的客户连接 %s\n", inet_ntoa(client.sin_addr));
		}
		struct epoll_event new_event;
		new_event.data.fd = fd;
		new_event.events = EPOLLIN;
		epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &new_event);

		//创建一个没有名字的玩家，并放入map中
		Player* player = new Player();
		socketMap[fd] = player;
		player->fd = fd;
	}
}

void handle_others(struct epoll_event ev, int epoll_fd)
{
	int fd = ev.data.fd;
	printf("处理客户%d的消息\n", fd);
	if(socketMap.find(fd) == socketMap.end())
	{
		printf("无效的客户端%d\n", fd);
		releasePlayer(fd);
		return;
	}
	Player* player = socketMap[fd];
	if(ev.events == EPOLLIN)
	{
		//读取新的消息
		if(player->remain_length == 0)
		{
			char buf[1];
			int len = recv(fd, buf, 1, 0);
			if(len == 0)
			{
				printf("玩家%s好像失联\n", player->playerName.data());
				ev.events = EPOLLIN;
				epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
				releasePlayer(fd);
				return;
			}
			if(len < 0)
			{
				ev.events = EPOLLIN;
				epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
				printf("玩家%s断开!\n", player->playerName.data());
				releasePlayer(fd);
				return;
			}
			//将数据读取到玩家的缓冲区中,并更新相应的计数
			else
			{
				player->remain_length = buf[0];
				len = read(fd, player->readBuf, player->remain_length);
				player->remain_length -= len;
				player->readBufLen += len;
			}

		}
		//否则继续读取上一次的数据
		else
		{
			int len = read(fd, player->readBuf + player->readBufLen, player->remain_length);
			if(len <= 0)
			{
				printf("玩家%s好像失联\n", player->playerName.data());
				ev.events = EPOLLIN;
				epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
				releasePlayer(fd);
				return;
			}
			player->remain_length -= len;
			player->readBufLen += len;
		}

		//读完了进行处理
		if(player->remain_length == 0)
		{
			player->readBuf[player->readBufLen] = '\0';
			if(player->readBufLen > 0)
			{
				handle_message(fd);
			}
			player->readBufLen = 0;
		}
	}
	else
	{
		printf("不同的事件类型%d\n", ev.events);
		ev.events = EPOLLIN;
		epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
		releasePlayer(fd);
	}
}

void releasePlayer(int fd)
{
	close(fd);
	if(socketMap.find(fd) == socketMap.end())
	{
		printf("玩家已经退出，%d\n", fd);
		return;
	}
	Player *player = socketMap[fd];
	if(player)
	{
		player->exitGame();
		socketMap.erase(fd);
		playerMap.erase(player->playerName);
		delete player;
	}
}

void handle_call_dizhu(Player* player)
{
	string msg(player->readBuf + 1);
	GameRoom *room = player->room;
	if(player->position == room->currentCallIndex)
	{
		//叫了地主 首先给其他人发送提示
		//然后提醒下一个人抢地主
		//回复叫地主成功或是失败
		sendMessage(player->fd, CALL_DIZHU_RESULT, msg);
		for(int i = 0; i < ROOM_SIZE; i++)
		{
			if(!room->players[i])
			{
				printf("ERROR:房间里玩家不够\n");
				break;
			}
			if(player != room->players[i])
			{
				//提醒其他玩家 格式为：叫或是不叫 玩家名称
				sendMessage(room->players[i]->fd,
						CALL_DIZHU_OTHERS, msg + player->playerName);
			}
		}
		if(msg == "1")
		{
			room->currentOutIndex = player->position;
		}

		room->currentRobCount++;
		//还可以继续叫地主
		if(room->currentRobCount < MAX_CALL_COUNT)
		{
			room->currentRobIndex = (room->currentCallIndex + 1) % ROOM_SIZE;
			room->currentCallIndex = -1;
		}
		//如果叫地主的次数够了 开始出牌
		else
		{
			room->currentRobIndex = -1;
			for(int i = 0; i < ROOM_SIZE; i++)
			{
				sendMessage(room->players[i]->fd, START_OUT_CARD,
					room->players[room->currentOutIndex]->playerName);
			}
		}
	}
	else
	{
		printf("Error:玩家%s不能叫地主\n", player->playerName.data());
		sendMessage(player->fd, CALL_DIZHU_RESULT, "0");
	}
}

void handle_rob_dizhu(Player* player)
{
	string msg(player->readBuf + 1);
	GameRoom* room = player->room;
	if(room->currentRobIndex == player->position)
	{
		//回复抢地主成功或是失败
		sendMessage(player->fd, CALL_DIZHU_RESULT, msg);
		for(int i = 0; i < ROOM_SIZE; i++)
		{
			if(!room->players[i])
			{
				printf("ERROR:房间里玩家不够\n");
				break;
			}
			if(player != room->players[i])
			{
				//提醒其他玩家 格式为：叫或是不叫 玩家名称
				sendMessage(room->players[i]->fd,
						CALL_DIZHU_OTHERS, msg + player->playerName);
			}
		}
		if(msg == "1")
		{
			room->currentOutIndex = player->position;
		}

		room->currentRobCount++;
		//还可以继续叫地主
		if(room->currentRobCount < MAX_CALL_COUNT)
		{
			room->currentRobIndex = (room->currentCallIndex + 1) % ROOM_SIZE;
			room->currentCallIndex = -1;
		}
		//如果叫地主的次数够了 开始出牌
		else
		{
			room->currentRobIndex = -1;
			for(int i = 0; i < ROOM_SIZE; i++)
			{
				sendMessage(room->players[i]->fd, START_OUT_CARD,
					room->players[room->currentOutIndex]->playerName);
			}
		}
	}
	else
	{
		printf("Error:玩家%s不能抢地主\n", player->playerName.data());
		sendMessage(player->fd, CALL_DIZHU_RESULT, "0");
	}
}
