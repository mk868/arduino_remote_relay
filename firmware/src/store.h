#include <IPAddress.h>

bool store_init();

IPAddress store_get_ip();

bool store_module_get_init_value(const char *path);

void store_module_set_init_value(const char *path, bool value);

void store_module_get_name(const char *id, Print &res);

void store_module_set_name(const char *id, const char *value);

bool store_file_exist(const char *path);

void store_read_file(const char *path, Print &res);

void store_write_file(const char *path, const char *value, uint8_t len);
