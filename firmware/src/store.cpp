#include "store.h"

#include <SD.h>
#include "config.h"

static const char *file_ip = "/ip";
static const char *store_dir = "store/";

bool store_init() {
    return SD.begin(SD_CS_PIN);
}

IPAddress store_get_ip() {
    uint8_t buffer[16];
    File file = SD.open(file_ip);
    auto size = file.read(buffer, sizeof(buffer) - 1);
    buffer[size] = 0;
    for (int i = 0; i < size; i++) {
        if (!(buffer[i] == '.' || (buffer[i] >= '0' && buffer[i] <= '9'))) {
            // when not allowed char -> limit that string
            buffer[i] = 0;
        }
    }
    IPAddress address;
    if (!address.fromString((char *) buffer)) {
        Serial.println("Parse IP error");
    }

    file.close();
    return address;
}

bool store_module_get_init_value(const char *id) {
    char buffer[20];
    strcpy(buffer, store_dir);
    strcat(buffer, id);
    strcat(buffer, ".v");
    if (!SD.exists(buffer)) {
        return false;
    }
    File file = SD.open(buffer);
    int ch = file.read();
    file.close();
    return ch == '1';
}

void store_module_set_init_value(const char *id, bool value) {
    char buffer[20];
    strcpy(buffer, store_dir);
    strcat(buffer, id);
    strcat(buffer, ".v");

    store_write_file(buffer, value ? "1" : "0", 1);
}

void store_module_get_name(const char *id, Print &res) {
    char buffer[20];
    strcpy(buffer, store_dir);
    strcat(buffer, id);
    strcat(buffer, ".n");

    store_read_file(buffer, res);
}

void store_module_set_name(const char *id, const char *value) {
    char buffer[20];
    strcpy(buffer, store_dir);
    strcat(buffer, id);
    strcat(buffer, ".n");

    store_write_file(buffer, value, strlen(value));
}

bool store_file_exist(const char *path) {
    return SD.exists(path);
}

void store_read_file(const char *path, Print &res) {
    File file = SD.open(path);
    int ch;
    while ((ch = file.read()) != -1) {
        res.write((uint8_t) ch);
    }
    file.close();
}

void store_write_file(const char *path, const char *value, uint8_t len) {
    File file = SD.open(path, O_WRITE | O_CREAT | O_TRUNC);
    file.write(value, len);
    file.close();
}
