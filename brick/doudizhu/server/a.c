#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <event.h>
#include<evhttp.h>

void root_handle(struct evhttp_request *req, void *arg)
{
	printf("Access root!");
	struct evbuffer *buf = evbuffer_new();
	if(buf == NULL)
	{
		perror("Failed to create response");
	}
	evbuffer_add_printf(buf, "Hello World!\n");
	evhttp_send_reply(req, HTTP_OK, "Ok", buf);
}

void other_handle(struct evhttp_request *req, void *arg)
{
	printf("Access other!");
	char tmp[1024];
	char output[2048] = "\0";
	const char *uri = evhttp_request_uri(req);
	sprintf(tmp, "uri=%s\n", uri);
	strcat(output, tmp);

	char *decode_uri = evhttp_decode_uri(uri);
	sprintf(tmp, "decode_uri=%s\n", decode_uri);
	strcat(output, tmp);

	struct evkeyvalq params;
	evhttp_parse_query(decode_uri, &params);
	sprintf(tmp, "q=%s\n", evhttp_find_header(&params, "q"));
	strcat(output, tmp);
	sprintf(tmp, "s=%s\n", evhttp_find_header(&params, "s"));
	strcat(output, tmp);
	free(decode_uri);

	char *post_data = (char *)EVBUFFER_DATA(req->input_buffer);
	sprintf(tmp, "post_data=%s\n", post_data);
	strcat(output, tmp);

	evhttp_add_header(req->output_headers, "Server", "localhost");
	evhttp_add_header(req->output_headers, "Content-Type", "text/plain; charset=UTF-8");
	evhttp_add_header(req->output_headers, "Connection", "close");

	struct evbuffer *buf = evbuffer_new();
	evbuffer_add_printf(buf, "Hello World\n%s\n", output);
	evhttp_send_reply(req, HTTP_OK, "Ok", buf);
	evbuffer_free(buf);

}
int main()
{
	struct evhttp *httpd;
	event_init();
	httpd = evhttp_start("0.0.0.0", 8989);
	evhttp_set_cb(httpd, "/", root_handle, NULL);
	evhttp_set_gencb(httpd, other_handle, NULL);

	event_dispatch();
	evhttp_free(httpd);
	return 0;
}
