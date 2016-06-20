#include "head.h"
#include <sys/types.h>
#include <event.h>
#include <evhttp.h>
#include <sstream>

extern map<string, GameRoom*> roomMap;
extern map<string, Player*> playerMap;
void message_handle(struct evhttp_request *req, void *arg);
void handle_illegal(struct evhttp_request *req, struct evkeyvalq *params);
void handle_get_room_list(struct evhttp_request* req, struct evkeyvalq *params);
void handle_enter_room(struct evhttp_request* req, struct evkeyvalq *params);
void handle_ready(struct evhttp_request* req, struct evkeyvalq *params);
void handle_out_card(struct evhttp_request* req, struct evkeyvalq *params);

int main()
{

}

void message_handle(struct evhttp_request *req, void *arg)
{
	int type = 0;
	char tmp[2048] = "\0";
	char *decode_uri = evhttp_decode_uri(req->uri);
	struct evkeyvalq params;
	evhttp_parse_query(decode_uri, &params);
	stringstream ss;
	ss << evhttp_find_header(&params, "type");
	ss >> type;
	switch(type){
	case GET_ROOM_LIST:
		handle_get_room_list(req, &params);
		break;
	case ENTER_ROOM:
		handle_enter_room(req, &params);
		break;
	case READY:
		handle_ready(req, &params);
		break;
	case OUT_CARD:
		handle_out_card(req, &params);
		break;
	default:
		handle_illegal(req, &params);
		break;
	}
}

void handle_get_room_list()
{

}
