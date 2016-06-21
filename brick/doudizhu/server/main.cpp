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
	int port = 7777;
	int listenfd;
	int epoll_fd;
	bool running = true;
	string host = "0.0.0.0";

	//设置监听套接字
	listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(listenfd <= 0)
	{
		perror("Failed to create socket!");
		return -1;
	}
	memset((char *)&server, 0 , sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(host.c_str());
	server.sin_port = htons(port);

	if(bind(listenfd, (struct sockaddr*)&server, sizeof(struct sockaddr)) != 0)
	{
		printf("Failed to bind in %s:%d", host.c_str(), port);
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

	struct epoll_event events[100];
	while(running)
	{
		int numbers = epoll_wait(epoll_fd, events, 100, -1);
		int i = 0;
		for(; i < numbers; i++)
		{
			if(events[i].data.fd == listenfd)
			{
				handle_accept(events[i], epoll_fd);
			}
			else
			{
				handle_others(events[i], epoll_fd);
			}
		}
	}
}

void sendMessage(int fd, int type, string message)
{
	int len = message.size();
	if(len > 253)
	{
		printf("发给%d的消息过长:%s\n", len, message.c_str());
		return;
	}
	char tmp[256];
	tmp[0] = len;
	tmp[1] = type;
	memcpy(tmp + 2, message.data(), len);
	send(fd, tmp, len, 0);
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
		printf("Player %d is out!\n", fd);
	}
	Player *player = socketMap[fd];
	int messageType = player->readBuf[0];
	switch(messageType){
	case GET_ROOM_LIST:
		handle_get_room_list(player);
		break;
	case ENTER_ROOM:
		handle_enter_room(player);
		break;
	case CREATE_ROOM:
		break;
	case READY:
		break;
	case OUT_CARD:
		break;
	}
	//清空玩家的缓冲区
	player->readBufLen = 0;

}

void handle_get_room_list(Player* player)
{
	sendRoomList(player->fd);
}

void handle_create_room(Player* player)
{
	//缓冲区的第一个字节是消息类型
	string name = player->readBuf + 1;
	if(roomMap.find(name) != roomMap.end())
	{
		printf("创建房间失败，房间%s已经存在\n", name.data());
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
				message1 += tmp->playerName + " ";
				stringstream ss;
				string str;
				ss << tmp->ready;
				ss >> str;
				message1.append(str + "\n");
			}
			if(tmp->fd != player->fd)
			{
				sendMessage(tmp->fd, ENTER_ROOM_OTHERS, message2);
			}
		}
		sendMessage(player->fd, ENTER_ROOM_RESULT, message1);
	}
}

void handle_ready(Player* player)
{
	player->ready = 1;
	sendMessage(player->fd, READY_RESULT, "");

	GameRoom* room = roomMap[player->roomName];
	if(!room)
	{
		printf("用户%s没有房间", player->playerName.data());
		return;
	}
	for(int i = 0; i < 3; i++)
	{
		Player* play = room->players[i];
		if(play && play->fd != player->fd)
		{
			sendMessage(play->fd, READY_OTHERS, player->playerName);
		}
	}
}

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
		struct epoll_event new_event;
		new_event.data.fd = fd;
		new_event.events = EPOLLIN;
		epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &new_event);
		sendRoomList(fd);
	}
}

void handle_others(struct epoll_event ev, int epoll_fd)
{
	int fd = ev.data.fd;
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
				printf("Client is down!\n");
			}
			//将数据读取到玩家的缓冲区中,并更新相应的计数
			else
			{
				player->remain_length = buf[0];
				len = read(fd, player->readBuf + player->readBufLen, player->remain_length);
				player->remain_length -= len;
				player->readBufLen += len;
			}

		}
		//否则继续读取上一次的数据
		else
		{
			int len = read(fd, player->readBuf + player->readBufLen, player->remain_length);
			player->remain_length -= len;
		}

		//读完了进行处理
		if(player->remain_length == 0)
		{
			player->readBuf[player->readBufLen] = '\0';
			if(player->readBufLen > 0)
			{
				handle_message(fd);
			}
		}
	}
}
