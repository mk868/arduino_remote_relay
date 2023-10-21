#include "store.h"

#include <SD.h>
#include "config.h"

static const char *file_ip = "/ip";

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