#include <IPAddress.h>

bool store_init();
IPAddress store_get_ip();
bool store_file_exist(const char * path);
void store_read_file(const char * path, Print &res);
