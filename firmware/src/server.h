#pragma once

#include <IPAddress.h>
#include <Print.h>

struct server_request_t
{
    String path;
    String query;
};

typedef void (*server_request_handler_t)(const server_request_t &request, Print &response);

bool server_init(const IPAddress& address);
void server_begin();
void server_loop();
void server_set_request_handler(server_request_handler_t cb);
