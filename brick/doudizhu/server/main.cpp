#include "head.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sstream>

void handle_accept(struct epoll_event, int epoll_fd);
void handle_others(struct epoll_event, int epoll_fd);
void sendMessage(int fd, int type, string message);
int main()
{
	struct sockaddr_in server;
	int listenfd;
	int epoll_fd;
	bool running = true;

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

	//初始化epoll
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

void sendMessage(int fd, int type, string message)
{
	printf("准备发送类型为%d的消息%s\n", type, message.data());
	int length = message.size() + 2;
	if(length >= 255)
	{
		printf("发给%d的消息过长:%s\n", fd, message.c_str());
		return;
	}
	message = (char)type + message;
	message = (char)length + message;

	length = 0;
	int res = 0;
	char *tmp = new char[message.size() + 1];
	strcat(tmp, message.data());
	tmp[message.size()] = '\0';
	while(length < (int)message.size())
	{
		res = send(fd, tmp + length, message.size() - length, 0);
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
				player->exitGame();
				socketMap.erase(fd);
				playerMap.erase(player->playerName);
				delete player;
			}
			delete[] tmp;
			return;
		}
		length += res;
	}
	delete[] tmp;
	printf("消息长度%d已经发送\n", (int)message.size());
}

void sendRoomList(int fd)
{
	string message = "";
	map<string, GameRoom *>::iterator ite = roomMap.begin();
	while(ite != roomMap.end())
	{
		message.append(ite->first);
		message.append(" ");
		stringstream ss;
		string str;
		ss << ite->second->currentPlayers;
		ss >> str;
		message.append(str);
		message.append(" ");

		ss.clear();
		ss << ite->second->isRunning;
		ss >> str;
		message.append(str);
		message.append("\n");
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
	printf("玩家%s, 消息类型%d\n", player->playerName.data(), messageType);
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

	printf("当前的用户有%ld个:", socketMap.size());
	map<int, Player*>::iterator ite = socketMap.begin();
	while(ite != socketMap.end())
	{
		string name = ite->second->playerName;
		printf("%s ", name.data());
		ite++;
	}
	printf("\n");
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
	string message = "0";
	if(room)
	{
		message = "1";
	}
	else
	{
		printf("加入房间%s失败\n", name.data());
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
		string message1 = "";
		string message2 = player->playerName;
		for(int i = 0; i < 3; i++)
		{
			Player* tmp = room->players[i];
			if(tmp)
			{
				if(tmp->fd != player->fd)
				{
					sendMessage(tmp->fd, ENTER_ROOM_OTHERS, message2);
				}
				message1 += tmp->playerName + " ";
				if(tmp->ready)
				{
					message1.append("1");
				}
				else
				{
					message1.append("0");
				}
				message1.append("\n");
			}
		}
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
	if(roomMap.find(player->roomName) == roomMap.end())
	{
		printf("用户%s没有房间%s\n", player->playerName.data(),
			player->roomName.data());
		return;
	}
	player->ready = true;
	sendMessage(player->fd, READY_RESULT, "");
	GameRoom* room = roomMap[player->roomName];
	for(int i = 0; i < 3; i++)
	{
		Player* play = room->players[i];
		if(play && play->fd != player->fd)
		{
			sendMessage(play->fd, READY_OTHERS, player->playerName);
		}
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
	if(socketMap.find(fd) == socketMap.end())
	{
		printf("无效的客户端%d\n", fd);
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
			while(len == 0)
			{
				len = recv(fd, buf, 1, 0);
			}
			if(len < 0)
			{
				ev.events = EPOLLIN;
				epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
				printf("玩家%s断开!\n", player->playerName.data());
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
}
