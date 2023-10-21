#include <IPAddress.h>

bool store_init();
IPAddress store_get_ip();
bool store_get_bool(const char * path);
void store_set_bool(const char * path, bool value);
bool store_file_exist(const char * path);
void store_read_file(const char * path, Print &res);
